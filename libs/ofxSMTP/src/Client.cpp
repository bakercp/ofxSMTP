// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofx/SMTP/Client.h"


namespace ofx {
namespace SMTP {



Client::Client():
    _isInited(false),
    _isSSLInited(false)
{
}


Client::~Client()
{
    _messageReady.set();

    if (getPocoThread().isRunning())
    {
        stopThread();
        getPocoThread().join(); // force the join
    }

    if(_isSSLInited)
    {
        // must be called once for each initializeSSL()
        Poco::Net::uninitializeSSL();
    }
}


void Client::setup(const Settings& settings)
{
    if (!_isInited)
    {
        _settings = settings;

        if (_settings.getEncryption().getType() != Encryption::NONE)
        {
            if(!_isSSLInited)
            {
                // may be called multiple times in a program, but must always
                // be paired with a call to Poco::Net::uninitilizeSSL(). 
                Poco::Net::initializeSSL();
                _isSSLInited = true;
                
                Poco::SharedPtr<Poco::Net::PrivateKeyPassphraseHandler> pConsoleHandler = new Poco::Net::KeyConsoleHandler(false);
                Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pInvalidCertHandler = new Poco::Net::ConsoleCertificateHandler(true);

                _pContext = settings.getEncryption().getContext();

                Poco::Net::SSLManager::instance().initializeClient(pConsoleHandler,
                                                                   pInvalidCertHandler,
                                                                   _pContext);
            }
        }

        _isInited = true;
    }
    else
    {
        ofLogError("Client::send") << "SMTP Client is already initialized.";
    }
}


void Client::send(const std::string& to,
                  const std::string& from,
                  const std::string& subject,
                  const std::string& body)
{
    Message::SharedPtr message = Message::makeShared();

    message->setSender(Poco::Net::MailMessage::encodeWord(from, "UTF-8"));
    message->addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, to));
    message->setSubject(Poco::Net::MailMessage::encodeWord(subject, "UTF-8"));
    message->setContentType("text/plain; charset=UTF-8");
    message->setContent(body, Poco::Net::MailMessage::ENCODING_8BIT);

    send(message);
}


void Client::send(Message::SharedPtr message)
{
    if(_isInited)
    {
        ofLogVerbose("Client::send") << "Pushing message to outbox.";

        mutex.lock();
        _outbox.push_back(message);
        mutex.unlock();

        // signal the thread
        _messageReady.set();

        // start the thread
        start();

    }
    else
    {
        ofLogError("Client::send") << "SMTP Client is not initialized.  Call setup().";
    }
}


void Client::threadedFunction()
{
    while(isThreadRunning())
    {
        Poco::Net::SocketAddress socketAddress(_settings.getHost(),
                                               _settings.getPort());
        SharedSocket pSocket;

        try
        {

            if(_settings.getEncryption().getType() == Encryption::SSLTLS)
            {
                pSocket = SharedSocket(new Poco::Net::SecureStreamSocket(socketAddress,
                                                                         _pContext));
            }
            else
            {
                pSocket = SharedSocket(new Poco::Net::StreamSocket(socketAddress));
            }
            
            Poco::Net::SecureSMTPClientSession session(*pSocket);
            
            session.setTimeout(_settings.getTimeout());
            
            if (Encryption::STARTTLS == _settings.getEncryption().getType())
            {
                // make the initial connection
                session.login();
                // this is supposed to return true on succes, but it doesn't.
                session.startTLS(_pContext);
            }

            if (_settings.getCredentials().getLoginMethod() !=
                Poco::Net::SMTPClientSession::AUTH_NONE)
            {
                session.login(_settings.getCredentials().getLoginMethod(),
                              _settings.getCredentials().getUsername(),
                              _settings.getCredentials().getPassword());
            }

            while (getOutboxSize() > 0 && isThreadRunning())
            {
                mutex.lock();
                _currentMessage = _outbox.front();
                _outbox.pop_front();
                mutex.unlock();

                session.sendMessage(*_currentMessage);

                ofNotifyEvent(events.onSMTPDelivery,
                              _currentMessage,
                              this);

                _currentMessage.reset();

                sleep(_settings.getMessageSendDelay().milliseconds());
            }

            ofLogVerbose("Client::threadedFunction") << "Closing session.";
            pSocket->close();

        }
        catch (Poco::Net::SMTPException& exc)
        {
            if (_currentMessage)
            {
                mutex.lock();
                _outbox.push_front(_currentMessage);
                _currentMessage.reset();
                mutex.unlock();
            }

            ofLogError("Client::threadedFunction") << exc.name() << " : " << exc.displayText();
            pSocket->close();
        }
        catch (Poco::Net::NetException& exc)
        {
            if (_currentMessage)
            {
                mutex.lock();
                _outbox.push_front(_currentMessage);
                _currentMessage.reset();
                mutex.unlock();
            }

            ofLogError("Client::threadedFunction") << exc.name() << " : " << exc.displayText();
            ofNotifyEvent(events.onSMTPException, exc, this);
        }
        catch (Poco::Net::SSLException& exc)
        {
            ofLogError("Client::threadedFunction") << exc.name() << " : " << exc.displayText();

            if (exc.displayText().find("SSL3_GET_SERVER_CERTIFICATE") != string::npos)
            {
                ofLogError("Client::threadedFunction") << "\t\t" << "This may be because you asked your SSL context to verify the server's certificate, but your certificate authority (ca) file is missing.";
            }

            ofNotifyEvent(events.onSMTPException, exc, this);
        }
        catch (Poco::Exception &exc)
        {
            ofLogError("Client::threadedFunction") << exc.name() << " : " << exc.displayText();
            ofNotifyEvent(events.onSMTPException, exc, this);
        }
        catch (std::exception& exc)
        {
            ofLogError("Client::threadedFunction") << exc.what();
            ofNotifyEvent(events.onSMTPException,
                          Poco::Exception(exc.what()),
                          this);
        }
        
        ofLogVerbose("Client::threadedFunction") << "Waiting for more messages.";

        _messageReady.wait();
        _messageReady.reset();
    }
}


std::size_t Client::getOutboxSize()
{
    ofScopedLock lock(mutex);
    return _outbox.size();
}


void Client::start()
{
    if (!isThreadRunning())
    {
        ofLogVerbose("Client::start") << "Starting thread.";
        startThread(true, false);   // blocking, verbose
    }
    else
    {
        ofLogVerbose("Client::start") << "New message queued, signalling.";
    }
}


} } // namespace ofx::SMTP

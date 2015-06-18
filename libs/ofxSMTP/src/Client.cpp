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
    _pSession(0),
    _isInited(false)
{
    ofAddListener(ofEvents().exit, this, &Client::exit);
}


Client::~Client()
{
    ofRemoveListener(ofEvents().exit, this, &Client::exit);
    waitForThread();
}


void Client::setup(const Settings& settings)
{
    if (!_isInited)
    {
        _settings = settings;
        _isInited = true;
    }
    else
    {
        ofLogError("Client::send") << "SMTP Client is already initialized.";
    }
}


void Client::exit(ofEventArgs& args)
{
    _messageReady.set();
    stopThread();
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
    if (_isInited)
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
    while (isThreadRunning())
    {
        Poco::Net::SocketAddress socketAddress(_settings.getHost(),
                                               _settings.getPort());
        SharedSocket pSocket;

        try
        {
            if (Settings::SSLTLS == _settings.getEncryptionType())
            {
                // Create a Poco::Net::SecureStreamSocket pointer.
                Poco::Net::SecureStreamSocket* _socket = 0;

                // We get a default context from ofSSLManager to make sure
                // that Poco::Net::SecureStreamSocket doesn't attempt to create
                // a default context using Poco::Net::SSLManager.
                Poco::Net::Context::Ptr _clientContext = ofSSLManager::getDefaultClientContext();

                // Create a Poco::Net::SecureStreamSocket and connect to it.
                // Use the Default Client context from ofSSLManager and
                // attempt to use saved Poco::Nett::Session on subsequent
                // attempts the Client SSL Context and server allow it,
                // and one was saved during a previous connection.
                _socket = new Poco::Net::SecureStreamSocket(socketAddress,
                                                            _clientContext,
                                                            _pSession);

                // Attempt to save an SSL Client session.
                _pSession = _socket->currentSession();

                // Let the shared pointer take ownership of the raw pointer.
                pSocket = SharedSocket(_socket);
            }
            else
            {
                pSocket = SharedSocket(new Poco::Net::StreamSocket(socketAddress));
            }

            #if defined(TARGET_OSX)
            // essential on early versions of Poco!  fixed in 1.4.6p2+ / 1.5.2+
            // https://github.com/pocoproject/poco/issues/235
            pSocket->setOption(SOL_SOCKET, SO_NOSIGPIPE, 1); // ignore SIGPIPE
            #endif

            Poco::Net::SecureSMTPClientSession session(*pSocket);
            
            session.setTimeout(_settings.getTimeout());
            
            if (Settings::STARTTLS == _settings.getEncryptionType())
            {
                // Make the initial connection.
                session.login();
                // TODO:
                // This is supposed to return true on succes, but it doesn't.
                session.startTLS(ofSSLManager::getDefaultClientContext());
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
            
            session.close();
            pSocket->close();

        }
        catch (Poco::Net::SMTPException& exc)
        {
            if (_currentMessage)
            {
                // 500 codes are permanent negative errors.
                if (5 != (exc.code() / 100))
                {
                    mutex.lock();
                    _outbox.push_front(_currentMessage);
                    mutex.unlock();
                }
            }

            pSocket->close();

            ErrorArgs args(exc, _currentMessage);
            ofNotifyEvent(events.onSMTPException,
                          args,
                          this);

            _currentMessage.reset();

        }
        catch (Poco::Net::SSLException& exc)
        {
            if (_currentMessage)
            {
                mutex.lock();
                _outbox.push_front(_currentMessage);
                mutex.unlock();
            }

            ofLogError("Client::threadedFunction") << exc.name() << " : " << exc.displayText();

            if (exc.displayText().find("SSL3_GET_SERVER_CERTIFICATE") != string::npos)
            {
                ofLogError("Client::threadedFunction") << "\t\t" << "This may be because you asked your SSL context to verify the server's certificate, but your certificate authority (ca) file is missing.";
            }

            ErrorArgs args(exc, _currentMessage);
            ofNotifyEvent(events.onSMTPException,
                          args,
                          this);

            _currentMessage.reset();
            
        }
        catch (Poco::Net::NetException& exc)
        {
            if (_currentMessage)
            {
                mutex.lock();
                _outbox.push_front(_currentMessage);
                mutex.unlock();
            }

            ofLogError("Client::threadedFunction") << exc.name() << " : " << exc.displayText();

                       ErrorArgs args(exc, _currentMessage);
            ofNotifyEvent(events.onSMTPException,
                          args,
                          this);

            _currentMessage.reset();
            
        }
        catch (Poco::Exception &exc)
        {
            if (_currentMessage)
            {
                mutex.lock();
                _outbox.push_front(_currentMessage);
                mutex.unlock();
            }

            ofLogError("Client::threadedFunction") << exc.name() << " : " << exc.displayText();

            ErrorArgs args(exc, _currentMessage);
            ofNotifyEvent(events.onSMTPException,
                          args,
                          this);

            _currentMessage.reset();
            
        }
        catch (std::exception& exc)
        {
            if (_currentMessage)
            {
                mutex.lock();
                _outbox.push_front(_currentMessage);
                mutex.unlock();
            }

            ofLogError("Client::threadedFunction") << exc.what();

            ErrorArgs args(Poco::Exception(exc.what()), _currentMessage);

            ofNotifyEvent(events.onSMTPException,
                          args,
                          this);

            _currentMessage.reset();
            
        }
        
        ofLogVerbose("Client::threadedFunction") << "Waiting for more messages.";

        _messageReady.wait();
        _messageReady.reset();
    }
}


std::size_t Client::getOutboxSize() const
{
    ofScopedLock lock(mutex);
    return _outbox.size();
}


void Client::start()
{
    if (!isThreadRunning())
    {
        ofLogVerbose("Client::start") << "Starting thread.";
        startThread(true);   // blocking, verbose
    }
    else
    {
        ofLogVerbose("Client::start") << "New message queued, signalling.";
    }
}


} } // namespace ofx::SMTP

//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/SMTP/Client.h"
#include "Poco/Net/MailMessage.h"


namespace ofx {
namespace SMTP {


Client::Client()
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
    auto message = std::make_shared<Poco::Net::MailMessage>();

    message->setSender(Poco::Net::MailMessage::encodeWord(from, "UTF-8"));
    message->addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, to));
    message->setSubject(Poco::Net::MailMessage::encodeWord(subject, "UTF-8"));
    message->setContentType("text/plain; charset=UTF-8");
    message->setContent(body, Poco::Net::MailMessage::ENCODING_8BIT);

    send(message);
}


void Client::send(std::shared_ptr<Poco::Net::MailMessage> message)
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
        using SS = Poco::Net::StreamSocket;
        using SSS = Poco::Net::SecureStreamSocket;
        using SMTP = Poco::Net::SMTPClientSession;
        using SSMTP = Poco::Net::SecureSMTPClientSession;
        using sSS = std::shared_ptr<SS>;
        using sSSS = std::shared_ptr<SSS>;
        using sSMTP = std::shared_ptr<SMTP>;

        sSMTP smtp = nullptr;

        try
        {
            if (Settings::SSLTLS == _settings.encryptionType())
            {
                ofLogVerbose("Client::threadedFunction") << "Settings::SSLTLS: " << _settings.host() << ":" << _settings.port();
                
                // Create a Poco::Net::SecureStreamSocket pointer.
                auto _socket = SSS(Poco::Net::SocketAddress(_settings.host(),
                                                            _settings.port()),
                                   _settings.host(),
                                   ofSSLManager::getDefaultClientContext(),
                                   _pSession);

                // Save the session for future use if possible.
                _pSession = _socket.currentSession();
                smtp = std::make_shared<SMTP>(_socket);
                smtp->setTimeout(_settings.timeout());
                smtp->login();
            }
            else if (Settings::STARTTLS == _settings.encryptionType())
            {
                ofLogVerbose("Client::threadedFunction") << "Settings::STARTTLS: " << _settings.host() << ":" << _settings.port();

                auto _smtp = std::make_shared<SSMTP>(_settings.host(),
                                                     _settings.port());
                
                _smtp->setTimeout(_settings.timeout());
                _smtp->login();

                ofLogVerbose("Client::threadedFunction") << "startTLS ...";
                if (!_smtp->startTLS(ofSSLManager::getDefaultClientContext()))
                {
                    ofLogWarning("Client::threadedFunction") << "startTLS failed.";
                }

                smtp = _smtp;
            }
            else
            {
                ofLogVerbose("Client::threadedFunction") << "Settings::NONE: " << _settings.host() << ":" << _settings.port();
                smtp = std::make_shared<SMTP>(_settings.host(), _settings.port());
                smtp->setTimeout(_settings.timeout());
                smtp->login();
            }

            ofLogVerbose("Client::threadedFunction") << "Setting timeout: " << _settings.timeout().totalMilliseconds();
            
            if (_settings.credentials().loginMethod() != Poco::Net::SMTPClientSession::AUTH_NONE)
            {
                try
                {
                    ofLogVerbose("Client::threadedFunction") << "Logging on with credentials.";
                    smtp->login(_settings.credentials().loginMethod(),
                                _settings.credentials().username(),
                                _settings.credentials().password());
                
                }
                catch (const Poco::Net::SMTPException& exc)
                {
                    ofLogError("Client::threadedFunction") << exc.displayText() << ": Check your ofxSMTP::Credentials.";
                    // There will likely be additional exceptions.
                }
            }

            while (getOutboxSize() > 0 && isThreadRunning())
            {
                mutex.lock();
                _currentMessage = _outbox.front();
                _outbox.pop_front();
                mutex.unlock();

                smtp->sendMessage(*_currentMessage);

                ofNotifyEvent(events.onSMTPDelivery, _currentMessage, this);

                _currentMessage.reset();

                sleep(_settings.messageSendDelay().milliseconds());
            }

            ofLogVerbose("Client::threadedFunction") << "Closing session.";
            
            if (smtp)
                smtp->close();
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

            if (smtp)
                smtp->close();

            ErrorArgs args(exc, _currentMessage);
            ofNotifyEvent(events.onSMTPException, args, this);

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

            if (exc.displayText().find("SSL3_GET_SERVER_CERTIFICATE") != std::string::npos)
            {
                ofLogError("Client::threadedFunction") << "\t\t" << "This may be because you asked your SSL context to verify the server's certificate, but your certificate authority (ca) file is missing.";
            }

            ErrorArgs args(exc, _currentMessage);
            ofNotifyEvent(events.onSMTPException, args, this);

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
            ofNotifyEvent(events.onSMTPException, args, this);

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

            ofNotifyEvent(events.onSMTPException, args, this);

            _currentMessage.reset();
            
        }

        _messageReady.wait();
        _messageReady.reset();
    }
}


std::size_t Client::getOutboxSize() const
{
    std::unique_lock<std::mutex> lock(mutex);
    return _outbox.size();
}

    
    
Settings Client::settings() const
{
    return _settings;
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

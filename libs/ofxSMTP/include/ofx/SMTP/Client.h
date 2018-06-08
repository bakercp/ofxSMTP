//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#undef verify // this is for OSX to get around the x509 macro error.


#include <string>
#include <deque>
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/SecureSMTPClientSession.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/StreamSocket.h"
#include "ofx/SMTP/Settings.h"
#include "ofx/SMTP/Events.h"
#include "ofLog.h"
#include "ofSSLManager.h"
#include "ofThread.h"


namespace ofx {
namespace SMTP {


/// \brief An SMTP Client.
class Client: public ofThread
{
public:
    /// \brief Create an SMTP client.
    Client();

    /// \brief Destroy an SMTP client.
    virtual ~Client();

    /// \brief Setup an SMTP client.
    /// \param settings The SMTP Client configuration.
    void setup(const Settings& settings = Settings());

    void exit(ofEventArgs& args);

    /// \brief Send a simple message with no attachments.
    /// \param to The recipient address.
    /// \param from The sender address.
    /// \param subject The subject of the message.
    /// \param body The plain text body of the message.
    void send(const std::string& to,
              const std::string& from,
              const std::string& subject,
              const std::string& body);

    /// \brief Send a more complex message with attachments etc.
    /// \param message The message to send.
    void send(std::shared_ptr<Poco::Net::MailMessage> message);

    /// \brief Get number in the outbox.
    /// \returns The number of messages queued in the outbox.
    std::size_t getOutboxSize() const; 

    /// \returns the current Settings.
    Settings settings() const;
    
    /// \brief The event callbacks.
    ClientEvents events;

    /// \brief Register a class to receive notifications for all events.
    /// \param listener a pointer to the listener class.
    /// \param priority the listener priority.
    /// \tparam ListenerClass The listener class.
    template <class ListenerClass>
    void registerEvents(ListenerClass* listener, int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofAddListener(events.onSMTPDelivery, listener, &ListenerClass::onSMTPDelivery, priority);
        ofAddListener(events.onSMTPException, listener, &ListenerClass::onSMTPException, priority);
    }
    
    /// \brief Unregister a class to receive notifications for all events.
    /// \param listener a pointer to the listener class.
    /// \param priority the listener priority.
    /// \tparam ListenerClass The listener class.
    template <class ListenerClass>
    void unregisterEvents(ListenerClass* listener, int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofRemoveListener(events.onSMTPDelivery, listener, &ListenerClass::onSMTPDelivery, priority);
        ofRemoveListener(events.onSMTPException, listener, &ListenerClass::onSMTPException, priority);
    }
    
private:
    /// \brief Start the thread.
    void start();

    /// \brief The threaded function.
    void threadedFunction();

    /// \brief The current client settings.
    Settings _settings;

    /// \brief The message outbox queue.
    std::deque<std::shared_ptr<Poco::Net::MailMessage>> _outbox;

    /// \brief The current message being sent.
    std::shared_ptr<Poco::Net::MailMessage> _currentMessage;

    /// \brief The send condition.
    Poco::Event _messageReady;

    /// \brief A session pointer to be set and reused if permitted.
    Poco::Net::Session::Ptr _pSession = nullptr;

    /// \brief Is the program initalized via setup?
    bool _isInited = false;

};


} } // namespace ofx::SMTP

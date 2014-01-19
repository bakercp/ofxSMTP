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
#include "ofx/SMTP/Types.h"
#include "ofx/SMTP/Events.h"


namespace ofx {
namespace SMTP {


class Client: public ofThread
    /// \brief An SMTP Client.
{
public:
    Client();
        ///< \brief Create an SMTP client.

    virtual ~Client();
        ///< \brief Destroy an SMTP client.

    void setup(const Settings& settings = Settings());
        ///< \brief Setup an SMTP client.
        ///< \param settings The SMTP Client configuration.
    
    void send(const std::string& to,
              const std::string& from,
              const std::string& subject,
              const std::string& body);
        ///< \brief Send a simple message with no attachments.
        ///< \param to The recipient address.
        ///< \param from The sender address.
        ///< \param subject The subject of the message.
        ///< \param body The plain text body of the message.
    
    void send(Message::SharedPtr message);
        ///< \brief Send a more complex message with attachments etc.
        ///< \param message The message to send.
    
    std::size_t getOutboxSize();
        ///< \brief Get number in the outbox.
        ///< \returns The number of messages queued in the outbox.
    
    ClientEvents events;
        ///< \brief The event callbacks.

private:
    typedef std::shared_ptr<Poco::Net::StreamSocket> SharedSocket;
        ///< \brief A typdef for a SharedSocket.

    void start();
        ///< \brief Start the thread.
    
    void threadedFunction();
        ///< \brief The threaded function.

    Settings _settings;
        ///< \brief The current client settings.

    std::deque<Message::SharedPtr> _outbox;
        ///< \brief The message outbox queue.

    Message::SharedPtr _currentMessage;
        ///< \brief THe current message being sent.

    Poco::Event _messageReady;
        ///< \brief The send condition.
    
    // SSL Related
    Poco::Net::Context::Ptr _pContext;
        ///< \brief the SSL context.

    bool _isInited;
    bool _isSSLInited;

};


} } // namespace ofx::SMTP

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
#include "ofSSLManager.h"


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
    void send(Message::SharedPtr message);

    /// \brief Get number in the outbox.
    /// \returns The number of messages queued in the outbox.
    std::size_t getOutboxSize();

    /// \brief The event callbacks.
    ClientEvents events;

private:
    /// \brief A typdef for a SharedSocket.
    typedef std::shared_ptr<Poco::Net::StreamSocket> SharedSocket;

    /// \brief Start the thread.
    void start();

    /// \brief The threaded function.
    void threadedFunction();

    /// \brief The current client settings.
    Settings _settings;

    /// \brief The message outbox queue.
    std::deque<Message::SharedPtr> _outbox;

    /// \brief THe current message being sent.
    Message::SharedPtr _currentMessage;

    /// \brief The send condition.
    Poco::Event _messageReady;

    /// \brief A session pointer to be set and reused if permitted.
    Poco::Net::Session::Ptr _pSession;

    /// \brief Is the program initalized via setup?
    bool _isInited;

};


} } // namespace ofx::SMTP

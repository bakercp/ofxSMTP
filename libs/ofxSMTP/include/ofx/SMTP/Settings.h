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


#include <string>
#include "Poco/Exception.h"
#include "Poco/Timespan.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/SMTPClientSession.h"
#include "ofUtils.h"
#include "ofxXmlSettings.h"
#include "ofx/SMTP/Credentials.h"
#include "ofx/SMTP/Encryption.h"


namespace ofx {
namespace SMTP {


class Settings
    /// \brief A base class representing SMTP Client settings.
{
public:
    Settings(const std::string& host = "",
             unsigned short port = DEFAULT_SMTP_PORT,
             Credentials credentials = Credentials(),
             Encryption encryption = Encryption(),
             Poco::Timespan timeout = DEFAULT_TIMEOUT,
             Poco::Timespan messageSendDelay = DEFAULT_MESSAGE_SEND_DELAY);
        ///< \brief Create SMTP Settings.
        ///< \param host The SMTP Server host.
        ///< \param port The SMTP Server port.
        ///< \param credentials The SMTP Credentials settings.
        ///< \param encryption The SMTP Encryption settings.
        ///< \param timeout The client timeout.
        ///< \param messageSendDelay The delay between sending messages.
    
    virtual ~Settings();
        ///< \brief Destroy the SMTP Settings.

    std::string getHost() const;
        ///< \returns The SMTP Server host.

    unsigned short getPort() const;
        ///< \returns The SMTP Server port.

    Credentials getCredentials() const;
        ///< \returns The SMTP Credential settings.

    Encryption getEncryption() const;
        ///< \returns The SMTP Encryption settings.

    Poco::Timespan getTimeout() const;
        ///< \returns The client timeout.

    Poco::Timespan getMessageSendDelay() const;
        ///< \returns The delay between sending message.

    static Settings loadFromXML(ofxXmlSettings xml,
                                const std::string& accountName = "");
        ///< \brief Load settings from an XML file.
        ///< \param xml The pre-parsed XML object.
        ///< \param accountName The account name within the XML file.
        ///< \note XML files can contain account information for multiple SMTP clients.
    
    static Settings loadFromXML(const std::string& filename,
                                const std::string& accountName = "");
        ///< \brief Load settings from an XML file.
        ///< \param filename The file name for the XML file.
        ///< \param accountName The account name within the XML file.
        ///< \note XML files can contain account information for multiple SMTP clients.

    static const Poco::Timespan DEFAULT_TIMEOUT;
        ///< \brief The default client timeout.
    static const Poco::Timespan DEFAULT_MESSAGE_SEND_DELAY;
        ///< \brief The delay between sending messages.

    enum
    {
        DEFAULT_SMTP_PORT = 25,
            ///< \brief Default SMTP Port.
        DEFAULT_SMTP_SSL_PORT = 425
            ///< \brief Default Secure SMTP Port.
    };

private:
    std::string _host;
        ///< \brief SMTP server host.

    unsigned short _port;
        ///< \brief SMTP server port.
    
    Credentials _credentials;
        ///< \brief Credentials settings.

    Encryption  _encryption;
        ///< \brief Encryption settings.

    Poco::Timespan _timeout;
        ///< \brief Client timeout.

    Poco::Timespan _messageSendDelay;
        ///< \brief The delay between sending messages.

};


class SSLTLSSettings: public Settings
    /// \brief A convenience class representing Secure SMTP Client settings.
{
public:
    SSLTLSSettings(const std::string& host,
                   unsigned short port = DEFAULT_SMTP_SSL_PORT,
                   Credentials credentials = Credentials());
        ///< \brief Create a SSLTLSSettings object.
        ///< \param host The SMTP Server host.
        ///< \param port The SMTP Server port.
        ///< \param credentials The SMTP Credentials settings.

    virtual ~SSLTLSSettings();
        ///< \brief Destroy the SSLTLSSettings.

};


} } // namespace ofx::SMTP

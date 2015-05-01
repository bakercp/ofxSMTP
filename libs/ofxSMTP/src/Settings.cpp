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


#include "ofx/SMTP/Settings.h"
#include "Poco/UTF8String.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/Util/XMLConfiguration.h"
#include "ofUtils.h"


namespace ofx {
namespace SMTP {


const Poco::Timespan Settings::DEFAULT_TIMEOUT = Poco::Timespan(30 * Poco::Timespan::SECONDS);
const Poco::Timespan Settings::DEFAULT_MESSAGE_SEND_DELAY= Poco::Timespan(100 * Poco::Timespan::MILLISECONDS);


Settings::Settings(const std::string& host,
                   unsigned short port,
                   Credentials credentials,
                   EncryptionType encryptionType,
                   Poco::Timespan timeout,
                   Poco::Timespan messageSendDelay):
    _host(host),
    _port(port),
    _credentials(credentials),
    _encryptionType(encryptionType),
    _timeout(timeout),
    _messageSendDelay(messageSendDelay)
{
}


Settings::~Settings()
{
}


std::string Settings::getHost() const
{
    return _host;
}


unsigned short Settings::getPort() const
{
    return _port;
}


Credentials Settings::getCredentials() const
{
    return _credentials;
}


Settings::EncryptionType Settings::getEncryptionType() const
{
    return _encryptionType;
}


Poco::Timespan Settings::getTimeout() const
{
    return _timeout;
}


Poco::Timespan Settings::getMessageSendDelay() const
{
    return _messageSendDelay;
}


Settings Settings::loadFromXML(const std::string& filename)
{
    try
    {
        Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration(ofToDataPath(filename, true)));
        return load(*pConf);
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("Settings::loadFromXML") << exc.displayText();
        return Settings();
    }
}


Settings Settings::load(const Poco::Util::AbstractConfiguration& config)
{
    std::string loginMethodString = config.getString("authentication.type", "AUTH_NONE");

    Credentials::LoginMethod loginMethod = Poco::Net::SMTPClientSession::AUTH_NONE;

    if (0 == Poco::UTF8::icompare(loginMethodString, "AUTH_NONE"))
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_NONE;
    }
    else if (0 == Poco::UTF8::icompare(loginMethodString, "AUTH_LOGIN"))
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_LOGIN;
    }
    else if (0 == Poco::UTF8::icompare(loginMethodString, "AUTH_CRAM_MD5"))
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_CRAM_MD5;
    }
    else if (0 == Poco::UTF8::icompare(loginMethodString, "AUTH_CRAM_SHA1"))
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_CRAM_SHA1;
    }
    else if (0 == Poco::UTF8::icompare(loginMethodString, "AUTH_PLAIN"))
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_PLAIN;
    }
    else
    {
        throw Poco::InvalidArgumentException("Unsupported authentication type: " + loginMethodString);
    }

    std::string encryptionTypeString = config.getString("encryption", "NONE");

    Settings::EncryptionType encryptionType = NONE;

    if (0 == Poco::UTF8::icompare(encryptionTypeString, "NONE"))
    {
        encryptionType = NONE;
    }
    else if (0 == Poco::UTF8::icompare(encryptionTypeString, "SSLTLS"))
    {
        encryptionType = SSLTLS;
    }
    else if (0 == Poco::UTF8::icompare(encryptionTypeString, "STARTTLS"))
    {
        encryptionType = STARTTLS;
    }
    else
    {
        throw Poco::InvalidArgumentException("Unsupported encryption type: " + encryptionTypeString);
    }

    return Settings(config.getString("host"),
                    config.getInt("port", 25),
                    Credentials(config.getString("authentication.username", ""),
                                config.getString("authentication.password", ""),
                                loginMethod),
                    encryptionType,
                    Poco::Timespan(config.getInt("timeout", 30000) * Poco::Timespan::MILLISECONDS),
                    Poco::Timespan(config.getInt("message-send-delay", 100) * Poco::Timespan::MILLISECONDS));
}


SSLTLSSettings::SSLTLSSettings(const std::string& host,
                               unsigned short port,
                               Credentials credentials):
    Settings(host,
             port,
             Credentials(credentials),
             Settings::SSLTLS)
{
}


SSLTLSSettings::~SSLTLSSettings()
{
}


STARTTLSSettings::STARTTLSSettings(const std::string& host,
                                   unsigned short port,
                                   Credentials credentials):
Settings(host,
         port,
         Credentials(credentials),
         Settings::STARTTLS)
{
}


STARTTLSSettings::~STARTTLSSettings()
{
}



} } // namespace ofx::SMTP


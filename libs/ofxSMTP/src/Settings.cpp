//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/SMTP/Settings.h"
#include "Poco/UTF8String.h"
#include "Poco/Version.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/Util/XMLConfiguration.h"
#if POCO_VERSION >= 0x01050000
    #include "Poco/Util/JSONConfiguration.h"
#endif
#include "ofLog.h"
#include "ofUtils.h"


namespace ofx {
namespace SMTP {


const Poco::Timespan Settings::DEFAULT_TIMEOUT = Poco::Timespan(30 * Poco::Timespan::SECONDS);
const Poco::Timespan Settings::DEFAULT_MESSAGE_SEND_DELAY= Poco::Timespan(100 * Poco::Timespan::MILLISECONDS);


Settings::Settings(const std::string& host,
                   uint16_t port,
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


std::string Settings::host() const
{
    return _host;
}


std::string Settings::getHost() const
{
    return host();
}


uint16_t Settings::port() const
{
    return _port;
}


uint16_t Settings::getPort() const
{
    return port();
}


Credentials Settings::credentials() const
{
    return _credentials;
}

    
Credentials Settings::getCredentials() const
{
    return credentials();
}

    
Settings::EncryptionType Settings::encryptionType() const
{
    return _encryptionType;
}


Settings::EncryptionType Settings::getEncryptionType() const
{
    return encryptionType();
}

    
Poco::Timespan Settings::timeout() const
{
    return _timeout;
}


Poco::Timespan Settings::getTimeout() const
{
    return timeout();
}

    
Poco::Timespan Settings::messageSendDelay() const
{
    return _messageSendDelay;
}


Poco::Timespan Settings::getMessageSendDelay() const
{
    return messageSendDelay();
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

//#if POCO_VERSION >= 0x01050000
//Settings Settings::loadFromJSON(const std::string& filename)
//{
//    try
//    {
//        Poco::AutoPtr<Poco::Util::JSONConfiguration> pConf(new Poco::Util::JSONConfiguration(ofToDataPath(filename, true)));
//
//        Poco::AutoPtr<Poco::Util::AbstractConfiguration> view= pConf->createView("account");
//
//        return load(*view);
//    }
//    catch (const Poco::Exception& exc)
//    {
//        ofLogError("Settings::loadFromJSON") << exc.displayText();
//        return Settings();
//    }
//}
//#endif


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
                               uint16_t port,
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
                                   uint16_t port,
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


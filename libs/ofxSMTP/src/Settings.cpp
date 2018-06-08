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


Settings Settings::fromJSON(const ofJson& json)
{
    Settings s;
    return s;
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
    
    
Settings Settings::loadFromJSON(const std::string& filename)
{
    try
    {
        Poco::AutoPtr<Poco::Util::JSONConfiguration> pConf(new Poco::Util::JSONConfiguration(ofToDataPath(filename, true)));
        return load(*pConf);
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("Settings::loadFromJSON") << exc.displayText();
        return Settings();
    }
}

    
Settings Settings::load(const Poco::Util::AbstractConfiguration& config)
{
    
    
    
    return Settings(config.getString("host"),
                    config.getUInt("port", DEFAULT_SMTP_PORT),
                    Credentials(config.getString("authentication.username", ""),
                                config.getString("authentication.password", ""),
                                Credentials::from_string(config.getString("authentication.type", "AUTH_NONE"))),
                    from_string(config.getString("encryption", "NONE")),
                    Poco::Timespan(config.getInt("timeout", 30000) * Poco::Timespan::MILLISECONDS),
                    Poco::Timespan(config.getInt("message-send-delay", 100) * Poco::Timespan::MILLISECONDS));
}
    
    
Settings::EncryptionType Settings::from_string(const std::string& method)
{
    if (method == "NONE")
    {
        return EncryptionType::NONE;
    }
    else if (method == "SSLTLS")
    {
        return EncryptionType::SSLTLS;
    }
    else if (method == "STARTTLS")
    {
        return EncryptionType::STARTTLS;
    }

    ofLogError("Settings::from_string") << "Unknown method: " << method;
    return EncryptionType::NONE;
}

    
std::string Settings::to_string(const Settings::EncryptionType& method)
{
    switch (method)
    {
        case NONE:
            return "NONE";
        case SSLTLS:
            return "SSLTLS";
        case STARTTLS:
            return "STARTTLS";
    }
    
    ofLogError("Settings::to_string") << "Unknown method: " << method;
    return "NONE";
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


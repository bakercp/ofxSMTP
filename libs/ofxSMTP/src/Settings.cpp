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



namespace ofx {
namespace SMTP {


const Poco::Timespan Settings::DEFAULT_TIMEOUT = Poco::Timespan(30 * Poco::Timespan::SECONDS);
const Poco::Timespan Settings::DEFAULT_MESSAGE_SEND_DELAY= Poco::Timespan(100 * Poco::Timespan::MILLISECONDS);


Settings::Settings(const std::string& host,
                   unsigned short port,
                   Credentials credentials,
                   Encryption encryption,
                   Poco::Timespan timeout,
                   Poco::Timespan messageSendDelay):
    _host(host),
    _port(port),
    _credentials(credentials),
    _encryption(encryption),
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


Encryption Settings::getEncryption() const
{
    return _encryption;
}


Poco::Timespan Settings::getTimeout() const
{
    return _timeout;
}


Poco::Timespan Settings::getMessageSendDelay() const
{
    return _messageSendDelay;
}


Settings Settings::loadFromXML(ofxXmlSettings xml, const std::string& accountName)
{
    // if account name is an empty string, then the first unnamed account will be matched
    // expected an <accounts> tag as current root element with child <account> elements

    Settings settings;

    int numAccounts = xml.getNumTags("account:accounts");
    
    int accountIndex = -1;
    
    for (std::size_t i = 0; i < numAccounts; ++i)
    {
        std::string _accountName = xml.getAttribute("account", "name", "", i);

        if (_accountName == accountName)
        {
            accountIndex = i;
            break;
        }
    }
    
    if (accountIndex < 0)
    {
        ofLogWarning("Settings::loadFromXML") << "No account called " << accountName << " found in XML file.";
        return settings;
    }
    
    std::string accountType = xml.getAttribute("account",
                                               "type",
                                               "",
                                               accountIndex);
    
    if ("smtp" != accountType && "SMTP" != accountType)
    {
        ofLogWarning("Settings::loadFromXML") << "Account type \"" << accountType << "\" is invalid.";
        return settings;
    }
    
    std::string host = xml.getValue("account:host", "", accountIndex);
    
    if (host.empty())
    {
        ofLogWarning("Settings::loadFromXML") << "Empty host field.";
        return settings;
    }
    
    unsigned short port = xml.getValue("account:port", 25, accountIndex);
    
    int timeoutInt = xml.getValue("account:timeout", 30000, accountIndex);
    
    Poco::Timespan timeout(timeoutInt * Poco::Timespan::MILLISECONDS);
    
    int messageSendDelayInt = xml.getValue("account:timeout", 100, accountIndex);
    
    Poco::Timespan messageSendDelay(messageSendDelayInt * Poco::Timespan::MILLISECONDS);
    
    std::string username = xml.getValue("account:authentication:username",
                                        "",
                                        accountIndex);

    std::string password = xml.getValue("account:authentication:password",
                                        "",
                                        accountIndex);

    std::string loginMethodString = xml.getValue("account:authentication:type",
                                                 "AUTH_NONE",
                                                 accountIndex);
    
    Credentials::LoginMethod loginMethod = Poco::Net::SMTPClientSession::AUTH_NONE;
    
    if ("AUTH_NONE" == loginMethodString)
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_NONE;
    }
    else if ("AUTH_LOGIN" == loginMethodString)
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_LOGIN;
    }
    else if ("AUTH_CRAM_MD5" == loginMethodString)
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_CRAM_MD5;
    }
    else if ("AUTH_CRAM_SHA1" == loginMethodString)
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_CRAM_SHA1;
    }
    else if ("AUTH_PLAIN" == loginMethodString)
    {
        loginMethod = Poco::Net::SMTPClientSession::AUTH_PLAIN;
    }
    else
    {
        ofLogWarning("Settings::loadFromXML") << "Unsupported authentication type: " << loginMethodString;
        return settings;
    }
    
    string encryptionTypeString = xml.getValue("account:encryption:type","NONE",accountIndex);
    
    Encryption::Type encryptionType;
    
    if ("NONE" == encryptionTypeString)
    {
        encryptionType = Encryption::NONE;
    }
    else if ("SSLTLS" == encryptionTypeString)
    {
        encryptionType = Encryption::SSLTLS;
    }
    else if ("STARTTLS" == encryptionTypeString)
    {
        encryptionType = Encryption::STARTTLS;
    }
    else
    {
        ofLogWarning("Settings::loadFromXML") << "Unsupported encyption type: " << encryptionTypeString;
        return settings;
    }
    
    std::string verificationModeString = xml.getValue("account:encryption:verification-mode",
                                                      "VERIFY_RELAXED",
                                                      accountIndex);
    
    Poco::Net::Context::VerificationMode verificationMode = Poco::Net::Context::VERIFY_RELAXED;
    
    if ("VERIFY_NONE" == verificationModeString)
    {
        verificationMode = Poco::Net::Context::VERIFY_NONE;
    }
    else if ("VERIFY_RELAXED" == verificationModeString)
    {
        verificationMode = Poco::Net::Context::VERIFY_RELAXED;
    }
    else if("VERIFY_STRICT" == verificationModeString)
    {
        verificationMode = Poco::Net::Context::VERIFY_STRICT;
    }
    else if ("VERIFY_ONCE" == verificationModeString)
    {
        verificationMode = Poco::Net::Context::VERIFY_ONCE;
    }
    else
    {
        ofLogWarning("Settings::loadFromXML") << "Unsupported verification type: " << verificationModeString;
        return settings;
    }
    
    std::string caLocation = xml.getValue("account:encryption:ca-location", "cacert.pem", accountIndex);
    
    
    settings = Settings(host,
                        port,
                        Credentials(username,
                                    password,
                                    loginMethod),
                        Encryption(encryptionType,
                                   caLocation,
                                   verificationMode),
                        timeout,
                        messageSendDelay);
    
    return settings;
}


Settings Settings::loadFromXML(const std::string& filename,
                               const std::string& accountName)
{
    ofxXmlSettings XML;

    if(XML.loadFile(filename))
    {
        XML.pushTag("accounts");
        return loadFromXML(XML,accountName);
    }
    else
    {
        ofLogError("Settings::loadFromXML") << "Unable to load XML from " << filename << ".  Loading defaults.";
        return Settings();
    }
}


SSLTLSSettings::SSLTLSSettings(const std::string& host,
                               unsigned short port,
                               Credentials credentials):
    Settings(host,
             port,
             Credentials(credentials),
             Encryption(Encryption::SSLTLS))
{
}


SSLTLSSettings::~SSLTLSSettings()
{
}



} } // namespace ofx::SMTP


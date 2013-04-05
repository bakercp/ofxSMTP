#include "ofxSMTPSettings.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ofxSMTPCredentials::ofxSMTPCredentials(const string& _username,
                                       const string& _password,
                                       const LoginMethod& _loginMethod) :
username(_username),
password(_password),
loginMethod(_loginMethod)
{ }

//------------------------------------------------------------------------------
ofxSMTPCredentials::~ofxSMTPCredentials() { }

//------------------------------------------------------------------------------
ofxSMTPEncryption::ofxSMTPEncryption(EncryptionType _type,
                                     Context::Ptr _context) :
type(_type),
context(_context)
{ }

//------------------------------------------------------------------------------
ofxSMTPEncryption::ofxSMTPEncryption(EncryptionType _encryptionType,
                                     const string& _caLocation,
                                     VerificationMode _verificationMode) :
type(_encryptionType)
{
    try {
        context = new Context(Context::CLIENT_USE,
                              ofToDataPath(_caLocation),
                              _verificationMode);
        
    } catch (const FileNotFoundException& exc) {
        ofLogError("ofxSMTPEncryption::ofxSMTPEncryption") << "caLocation not found: " << ofToDataPath(_caLocation) << ". Using none.";
        if(_verificationMode != Context::VERIFY_NONE) {
            ofLogError("ofxSMTPEncryption::ofxSMTPEncryption") << "\t\tSince you requested certificate verification, encrypted calls using this context will fail.";
        }
        context = new Context(Context::CLIENT_USE,
                              "",
                              _verificationMode);
    }
}

//------------------------------------------------------------------------------
ofxSMTPEncryption::~ofxSMTPEncryption() { }

//------------------------------------------------------------------------------
ofxSMTPSettings::ofxSMTPSettings(const string& _host,
                                 unsigned short _port,
                                 ofxSMTPCredentials _credentials,
                                 ofxSMTPEncryption _encryption,
                                 Timespan _timeout,
                                 Timespan _messageSendDelay)
:
host(_host),
port(_port),
credentials(_credentials),
encryption(_encryption),
timeout(_timeout),
messageSendDelay(_messageSendDelay)
{ }

//------------------------------------------------------------------------------
ofxSMTPSettings::~ofxSMTPSettings() { }

//------------------------------------------------------------------------------
ofxSMTPSettings ofxSMTPSettings::loadFromXML(ofxXmlSettings xml, const string& accountName) {
    // if account name is an empty string, then the first unnamed account will be matched
    // expected an <accounts> tag as current root element with child <account> elements
    ofxSMTPSettings settings;
    int numAccounts = xml.getNumTags("account:accounts");
    
    int accountIndex = -1;
    
    for(int i = 0; i < numAccounts; i++) {
        string _accountName = xml.getAttribute("account", "name", "", i);
        if(_accountName == accountName) {
            accountIndex = i;
            break;
        }
    }
    
    if(accountIndex < 0) {
        ofLogWarning("ofxSMTPSettings::loadFromXML") << "No account called " << accountName << " found in XML file.";
        return settings;
    }
    
    string accountType = xml.getAttribute("account","type","",accountIndex);
    
    if(accountType != "smtp" && accountType != "SMTP") {
        ofLogWarning("ofxSMTPSettings::loadFromXML") << "Account type \"" << accountType << "\" is invalid.";
        return settings;
    }
    
    string host = xml.getValue("account:host","",accountIndex);
    
    if(host.empty()) {
        ofLogWarning("ofxSMTPSettings::loadFromXML") << "Empty host field.";
        return settings;
    }
    
    unsigned short port = xml.getValue("account:port",25,accountIndex);
    
    int timeoutInt = xml.getValue("account:timeout",30000,accountIndex);
    
    Timespan timeout(timeoutInt * Timespan::MILLISECONDS);
    
    int messageSendDelayInt = xml.getValue("account:timeout",100,accountIndex);
    
    Timespan messageSendDelay(messageSendDelayInt * Timespan::MILLISECONDS);
    
    string username = xml.getValue("account:authentication:username","",accountIndex);
    string password = xml.getValue("account:authentication:password","",accountIndex);
    
    string loginMethodString = xml.getValue("account:authentication:type","AUTH_NONE",accountIndex);
    
    ofxSMTPCredentials::LoginMethod loginMethod = SMTPClientSession::AUTH_NONE;
    
    if(loginMethodString == "AUTH_NONE") {
        loginMethod = SMTPClientSession::AUTH_NONE;
    } else if(loginMethodString == "AUTH_LOGIN") {
        loginMethod = SMTPClientSession::AUTH_LOGIN;
    } else if(loginMethodString == "AUTH_CRAM_MD5") {
        loginMethod = SMTPClientSession::AUTH_CRAM_MD5;
    } else if(loginMethodString == "AUTH_CRAM_SHA1") {
        loginMethod = SMTPClientSession::AUTH_CRAM_SHA1;
    } else if(loginMethodString == "AUTH_PLAIN") {
        loginMethod = SMTPClientSession::AUTH_PLAIN;
    } else {
        ofLogWarning("ofxSMTPSettings::loadFromXML") << "Unsupported authentication type: " << loginMethodString;
        return settings;
    }
    
    string encryptionTypeString = xml.getValue("account:encryption:type","NONE",accountIndex);
    
    ofxSMTPEncryption::EncryptionType encryptionType;
    
    if(encryptionTypeString == "NONE") {
        encryptionType = ofxSMTPEncryption::NONE;
    } else if(encryptionTypeString == "SSLTLS") {
        encryptionType = ofxSMTPEncryption::SSLTLS;
    } else if(encryptionTypeString == "STARTTLS") {
        encryptionType = ofxSMTPEncryption::STARTTLS;
    } else {
        ofLogWarning("ofxSMTPSettings::loadFromXML") << "Unsupported encyption type: " << encryptionTypeString;
        return settings;
    }
    
    string verificationModeString = xml.getValue("account:encryption:verification-mode","VERIFY_RELAXED",accountIndex);
    
    Context::VerificationMode verificationMode = Context::VERIFY_RELAXED;
    
    if(verificationModeString == "VERIFY_NONE") {
        verificationMode = Context::VERIFY_NONE;
    } else if(verificationModeString == "VERIFY_RELAXED") {
        verificationMode = Context::VERIFY_RELAXED;
    } else if(verificationModeString == "VERIFY_STRICT") {
        verificationMode = Context::VERIFY_STRICT;
    } else if(verificationModeString == "VERIFY_ONCE") {
        verificationMode = Context::VERIFY_ONCE;
    } else {
        ofLogWarning("ofxSMTPSettings::loadFromXML") << "Unsupported verification type: " << verificationModeString;
        return settings;
    }
    
    string caLocation = xml.getValue("account:encryption:ca-location","cacert.pem", accountIndex);
    
    
    settings = ofxSMTPSettings(host,
                               port,
                               ofxSMTPCredentials(username,password,loginMethod),
                               ofxSMTPEncryption(encryptionType,
                                                 caLocation,
                                                 verificationMode),
                               timeout,
                               messageSendDelay);
    
    return settings;
}

//------------------------------------------------------------------------------
ofxSMTPSettings ofxSMTPSettings::loadFromXML(const string& filename, const string& accountName) {
    ofxXmlSettings XML;
    if(XML.loadFile(filename)) {
        XML.pushTag("accounts");
        return loadFromXML(XML,accountName);
    } else {
        ofxSMTPSettings settings;
        ofLogError("ofxSMTPSettings::loadFromXML") << "Unable to load XML from " << filename << ".  Loading defaults.";
        return settings;
    }
}


//------------------------------------------------------------------------------
ofxSMTPSSLTLSSettings::ofxSMTPSSLTLSSettings(const string& _host,
                                             unsigned short _port,
                                             ofxSMTPCredentials _credentials) :
ofxSMTPSettings(_host,
                _port,
                ofxSMTPCredentials(_credentials),
                ofxSMTPEncryption(ofxSMTPEncryption::SSLTLS))
{ }

//------------------------------------------------------------------------------
ofxSMTPSSLTLSSettings::~ofxSMTPSSLTLSSettings() { }

//------------------------------------------------------------------------------
ofxSMTPGmailSettings::ofxSMTPGmailSettings(const string& username,
                                           const string& password) :
ofxSMTPSSLTLSSettings("smtp.gmail.com",
                      465,
                      ofxSMTPCredentials(username,
                                         password,
                                         SMTPClientSession::AUTH_LOGIN))
{ }

//------------------------------------------------------------------------------
ofxSMTPGmailSettings::~ofxSMTPGmailSettings() { }



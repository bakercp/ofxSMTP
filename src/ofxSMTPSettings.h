/*==============================================================================
 
 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 =============================================================================*/

#pragma once

#include <string>

#include "Poco/Exception.h"
#include "Poco/Timespan.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/SMTPClientSession.h"

#include "ofUtils.h"
#include "ofxXmlSettings.h"

using std::string;

using Poco::FileNotFoundException;
using Poco::Timespan;
using Poco::Net::Context;
using Poco::Net::SMTPClientSession;

//------------------------------------------------------------------------------
class ofxSMTPCredentials {
public:
    typedef SMTPClientSession::LoginMethod LoginMethod;

    ofxSMTPCredentials(const string& _username = "",
                       const string& _password = "",
                       const LoginMethod& _loginMethod = SMTPClientSession::AUTH_NONE);
    
    virtual ~ofxSMTPCredentials();
    
    string username;
    string password;
    LoginMethod loginMethod;
    
};

//------------------------------------------------------------------------------
class ofxSMTPEncryption {
public:
    typedef Context::VerificationMode VerificationMode;
    
    enum EncryptionType {
        NONE,
        SSLTLS,
        STARTTLS
    };
    
    ofxSMTPEncryption(EncryptionType _type, Context::Ptr _context);
    ofxSMTPEncryption(EncryptionType _encryptionType = NONE,
                      const string& _caLocation = "cacert.pem",
                      VerificationMode _verificationMode = Context::VERIFY_RELAXED);

    virtual ~ofxSMTPEncryption();
    
    EncryptionType type;
    Context::Ptr context;
    
};

//------------------------------------------------------------------------------
class ofxSMTPSettings {
public:
    
    enum Encryption {
        NONE,
        SSLTLS,
        STARTTLS
    };
    
    ofxSMTPSettings(const string& _host = "",
                    unsigned short _port = 25,
                    ofxSMTPCredentials _credentials = ofxSMTPCredentials(),
                    ofxSMTPEncryption _encryption = ofxSMTPEncryption(),
                    Timespan _timeout = Timespan(30 * Timespan::SECONDS),
                    Timespan _messageSendDelay = Timespan(100 * Timespan::MILLISECONDS));
    
    virtual ~ofxSMTPSettings();
    
    static ofxSMTPSettings loadFromXML(ofxXmlSettings xml,
                                       const string& accountName = "");
    
    static ofxSMTPSettings loadFromXML(const string& filename,
                                       const string& accountName = "");
    
    string host;
    unsigned short port;
    
    ofxSMTPCredentials credentials;
    ofxSMTPEncryption  encryption;
    
    Timespan timeout;
    Timespan messageSendDelay;
    
};

//------------------------------------------------------------------------------
class ofxSMTPSSLTLSSettings : public ofxSMTPSettings {
public:
    ofxSMTPSSLTLSSettings(const string& _host,
                          unsigned short _port = 465,
                          ofxSMTPCredentials _credentials = ofxSMTPCredentials());

    virtual ~ofxSMTPSSLTLSSettings();
};

//------------------------------------------------------------------------------
class ofxSMTPGmailSettings : public ofxSMTPSSLTLSSettings {
public:
    ofxSMTPGmailSettings(const string& username, const string& password);
    virtual ~ofxSMTPGmailSettings();

    // Note: Gmail also works with port = 587 and
    // encryption.type = ofxSMTPEncryption::STARTTLS.
    // Since this port is often blocked, we prefer 465 with SSL/TLS.
    
};


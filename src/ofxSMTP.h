// =============================================================================
//
// Copyright (c) 2010-2013 Christopher Baker <http://christopherbaker.net>
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
#include <queue>

#include "Poco/Condition.h"
#include "Poco/Exception.h"
#include "Poco/SharedPtr.h"
#include "Poco/Timespan.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/SecureSMTPClientSession.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/StreamSocket.h"

#include "ofLog.h"
#include "ofTypes.h"
#include "ofThread.h"
#include "ofUtils.h"

#include "ofxSMTPSettings.h"

using std::string;
using std::queue;

using Poco::Condition;
using Poco::Exception;
using Poco::SharedPtr;
using Poco::Timespan;
using Poco::Net::ConsoleCertificateHandler;
using Poco::Net::Context;
using Poco::Net::InvalidCertificateHandler;
using Poco::Net::KeyConsoleHandler;
using Poco::Net::MailMessage;
using Poco::Net::MailRecipient;
using Poco::Net::NetException;
using Poco::Net::PrivateKeyPassphraseHandler;
using Poco::Net::SMTPException;
using Poco::Net::SecureSMTPClientSession;
using Poco::Net::SecureStreamSocket;
using Poco::Net::SocketAddress;
using Poco::Net::SSLException;
using Poco::Net::SSLManager;
using Poco::Net::StreamSocket;

typedef ofPtr<MailMessage> ofxMailMessage;

class ofxSMTPEventArgs {
public:
    ofxSMTPEventArgs(ofxMailMessage _message, bool _delivered) :
    message(_message),
    delivered(_delivered)
    { }
    
    bool delivered;
    ofxMailMessage message;
};

class ofxSMTPEvents {
public:
    ofEvent<ofxSMTPEventArgs> status;
    // progress once Poco supports it
    // http://pocoproject.org/forum/viewtopic.php?f=12&t=5655&p=9788&hilit=smtp#p9788
    
};


class ofxSMTP : public ofThread {
public:
    typedef ofxSMTPCredentials Credentials;
    typedef ofxSMTPSettings    Settings;
    
    ofxSMTP();
    virtual ~ofxSMTP();
    
    void setup(Settings _settings = Settings());
    
    // simple
    void send(const string& to,
              const string& from,
              const string& subject,
              const string& content);
    
    // full featured
    void send(ofxMailMessage message);
    
    size_t getNumQueued();
    
    ofxSMTPEvents events;
    
private:
    void start();
    
    void threadedFunction();
       
    Settings settings;
    bool bIsInited;
    bool bIsSSLInited;
    
    queue<ofxMailMessage> outbox;
    
    unsigned long long lastMessageSent;
    
    Condition condition;
    
    // SSL Related
    Context::Ptr pContext;
    
};
#include "ofxSMTP.h"

//------------------------------------------------------------------------------
ofxSMTP::ofxSMTP() :
bIsInited(false),
bIsSSLInited(false),
lastMessageSent(0)
{ }

//------------------------------------------------------------------------------
ofxSMTP::~ofxSMTP() {
    if(bIsSSLInited) {
        // must be called once for each initializeSSL()
        Poco::Net::uninitializeSSL();
    }
}

//------------------------------------------------------------------------------
void ofxSMTP::setup(Settings _settings) {
    if(!bIsInited) {
        settings = _settings;
        if(settings.encryption.type != ofxSMTPEncryption::NONE) {
            
            if(!bIsSSLInited) {
                // may be called multiple times in a program, but must always
                // be paired with a call to Poco::Net::uninitilizeSSL(). 
                Poco::Net::initializeSSL();
                bIsSSLInited = true;
                
                SharedPtr<PrivateKeyPassphraseHandler> pConsoleHandler = new KeyConsoleHandler(false);
                SharedPtr<InvalidCertificateHandler> pInvalidCertHandler = new ConsoleCertificateHandler(true);
                pContext = settings.encryption.context;

                SSLManager::instance().initializeClient(pConsoleHandler, pInvalidCertHandler, pContext);
            }
        }
        bIsInited = true;
    } else {
        ofLogError("ofxSMTP::send") << "ofxSMTP is already initialized.";
    }
}

//------------------------------------------------------------------------------
void ofxSMTP::send(const string& to,
                   const string& from,
                   const string& subject,
                   const string& content) {
    
    ofxMailMessage message = ofxMailMessage(new MailMessage());
    message->setSender(MailMessage::encodeWord(from, "UTF-8"));
    message->addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
    message->setSubject(MailMessage::encodeWord(subject, "UTF-8"));
    message->setContentType("text/plain; charset=UTF-8");
    message->setContent(content, MailMessage::ENCODING_8BIT);
    send(message);
}

//------------------------------------------------------------------------------
void ofxSMTP::send(ofxMailMessage message) {
    if(bIsInited) {
        ofLogVerbose("ofxSMTP::send") << "Pushing message to outbox.";
        mutex.lock();
        outbox.push(message);
        condition.signal();
        mutex.unlock();
        start();
    } else {
        ofLogError("ofxSMTP::send") << "ofxSMTP is not initialized.  Call setup().";
    }
}

//------------------------------------------------------------------------------
void ofxSMTP::threadedFunction() {
    while(isThreadRunning()) {
        
        ofxMailMessage outgoingMessage;
        
        try {
            
            SocketAddress sa(settings.host, settings.port);
            
            StreamSocket* socketPtr = NULL;
            SecureSMTPClientSession* sessionPtr = NULL;

            if(settings.encryption.type == ofxSMTPEncryption::SSLTLS) {
                socketPtr = new SecureStreamSocket(sa,pContext);
            } else {
                socketPtr = new StreamSocket(sa);
            }
            
            sessionPtr = new SecureSMTPClientSession(*socketPtr);
            
            sessionPtr->setTimeout(settings.timeout);
            
            try {
                
                if(settings.encryption.type == ofxSMTPEncryption::STARTTLS) {
                    sessionPtr->login(); // make the initial connection
                    sessionPtr->startTLS(pContext); // this is supposed to return true on succes, but it doesn't.
                }
                
                if(settings.credentials.loginMethod != SMTPClientSession::AUTH_NONE) {
                    sessionPtr->login(settings.credentials.loginMethod,
                                      settings.credentials.username,
                                      settings.credentials.password);
                }
                
                while(getNumQueued() > 0 && isThreadRunning()) {
                    unsigned long long timeSinceLastMessageSent = ofGetElapsedTimeMillis() - lastMessageSent;
                    if(timeSinceLastMessageSent > settings.messageSendDelay.milliseconds()) {
                        mutex.lock();
                        outgoingMessage = outbox.front();
                        ofLogVerbose("ofxSMTP::threadedFunction") << "Sending : " << outgoingMessage->getSubject();;
                        outbox.pop();
                        mutex.unlock();
                        sessionPtr->sendMessage(*outgoingMessage.get());
                        lastMessageSent = ofGetElapsedTimeMillis();
                        ofxSMTPEventArgs args(outgoingMessage,true);
                        ofNotifyEvent(events.status, args);
                    } else {
                        yield();
                    }
                }

                ofLogVerbose("ofxSMTP::threadedFunction") << "Closing session.";

                sessionPtr->close();
                delete sessionPtr;
                delete socketPtr;
            } catch (const SMTPException& exc) {
                ofLogError("ofxSMTP::threadedFunction") << exc.name() << " : " << exc.displayText();
                sessionPtr->close();
                delete sessionPtr;
                delete socketPtr;
                
                ofxSMTPEventArgs args(outgoingMessage,false);
                ofNotifyEvent(events.status, args);
            }
        } catch (const SSLException &exc) {
            ofLogError("ofxSMTP::threadedFunction") << exc.name() << " : " << exc.displayText();

            if(exc.displayText().find("SSL3_GET_SERVER_CERTIFICATE") != string::npos) {
                ofLogError("ofxSMTP::threadedFunction") << "\t\t" << "This may be because you asked your SSL context to verify the server's certificate, but your certificate authority (ca) file is missing.";
            } 
            
            ofxSMTPEventArgs args(outgoingMessage,false);
            ofNotifyEvent(events.status, args);
        } catch (const NetException &exc) {
            ofLogError("ofxSMTP::threadedFunction") << exc.name() << " : " << exc.displayText();
            ofxSMTPEventArgs args(outgoingMessage,false);
            ofNotifyEvent(events.status, args);
        } catch (const Exception &exc) {
            ofLogError("ofxSMTP::threadedFunction") << exc.name() << " : " << exc.displayText();
            ofxSMTPEventArgs args(outgoingMessage,false);
            ofNotifyEvent(events.status, args);
        } catch(const std::exception& exc) {
            ofLogError("ofxSMTP::threadedFunction") << exc.what();
            ofxSMTPEventArgs args(outgoingMessage,false);
            ofNotifyEvent(events.status, args);
        }
        
        ofLogVerbose("ofxSMTP::threadedFunction") << "Waiting for more messages.";
        mutex.lock();
        condition.wait(mutex);
        mutex.unlock();
    }
}

//------------------------------------------------------------------------------
size_t ofxSMTP::getNumQueued() {
    ofScopedLock lock(mutex);
    return outbox.size();
}

//------------------------------------------------------------------------------
void ofxSMTP::start() {
    if (!isThreadRunning()){
        ofLogVerbose("ofxSMTP::send") << "Starting thread.";
        startThread(true, false);   // blocking, verbose
    }else{
        ofLogVerbose("ofxSMTP::threadedFunction") << "New message queued, signalling.";
        condition.signal();
    }
}

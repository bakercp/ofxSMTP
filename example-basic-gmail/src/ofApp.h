//
// Copyright (c) 2010 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxSMTP.h"


class ofApp: public ofBaseApp
{
public:
    void setup();
    void draw();
    
    void keyPressed(int key);

    void onSMTPDelivery(std::shared_ptr<Poco::Net::MailMessage>& message);
    void onSMTPException(const ofxSMTP::ErrorArgs& evt);

    void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLPrivateKeyPassphraseRequired(std::string& passphrase);

    ofxSMTP::Client smtp;

};

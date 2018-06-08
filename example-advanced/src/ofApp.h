//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxSMTP.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;
    void keyPressed(int key) override;

    void onSMTPDelivery(std::shared_ptr<Poco::Net::MailMessage>& message);
    void onSMTPException(const ofxSMTP::ErrorArgs& evt);

    void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLPrivateKeyPassphraseRequired(std::string& passphrase);

    ofEventListener smtpDeliveryListener;
    ofEventListener smtpExceptionListener;

    std::string recipientEmail;
    std::string senderEmail;

    ofxSMTP::Client smtp;

};

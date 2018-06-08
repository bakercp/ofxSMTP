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
    void setup() override;
    void draw() override;
    void keyPressed(int key) override;

    void onSMTPDelivery(std::shared_ptr<Poco::Net::MailMessage>& message);
    void onSMTPException(const ofxSMTP::ErrorArgs& evt);

    ofxSMTP::Client smtp;

};

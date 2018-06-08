//
// Copyright (c) 2010 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Optionally register for SMTP events. Requires the
    // onSMTPDelivery(std::shared_ptr<Poco::Net::MailMessage>& message) and
    // onSMTPException(const ofxSMTP::ErrorArgs& evt) to be pressent.
    smtp.registerEvents(this);
    
    // Use the simple gmail settings (also works for any gmail based account).
    // It is recomended to use a Gmail app password. More information can be
    // found here: https://myaccount.google.com/apppasswords
    // See ofx::SMTP::Settings for more extensive configuration options.
    ofxSMTP::GmailSettings settings("USERNAME@gmail.com","YOUR_GMAIL_APP_PASSWORD");

    // Pass the settings to the client.
    smtp.setup(settings);
}


void ofApp::draw()
{
    // Print some information about the state of the outbox.
    ofBackground(80);
    ofDrawBitmapStringHighlight("         Press <SPACEBAR> to Send\n\nofxSMTP: There are " + ofToString(smtp.getOutboxSize()) + " messages in your outbox.", 10, 20);
}


void ofApp::keyPressed(int key)
{
    if(key == ' ')
    {
        // Send a simple short message.
        smtp.send("info@christopherbaker.net", // Recipient email.
                  "USERNAME@gmail.com",        // Sender email.
                  "I'm trying out ofxSMTP!",   // Subject line.
                  "It works!");                // Message body.
    }
}


void ofApp::onSMTPDelivery(std::shared_ptr<Poco::Net::MailMessage>& message)
{
    ofLogNotice("ofApp::onSMTPDelivery") << "Message Sent: " << message->getSubject();
}


void ofApp::onSMTPException(const ofxSMTP::ErrorArgs& evt)
{
    ofLogError("ofApp::onSMTPException") << evt.error().displayText();

    if (evt.message())
    {
        ofLogError("ofApp::onSMTPException") << evt.message()->getSubject();
    }
}

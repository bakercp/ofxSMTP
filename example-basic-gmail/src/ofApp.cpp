//
// Copyright (c) 2010 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Register for SSL Context events.
    ofSSLManager::registerClientEvents(this);

    // Use the simple gmail settings (also works for any gmail based account).
    ofx::SMTP::GmailSettings settings("USERNAME@gmail.com","YOUR_GMAIL_PASSWORD");

    // See SMTP::Settings for extensive configuration options.

    // Pass the settings to the client.
    smtp.setup(settings);

    // Register event callbacks for message delivery (or failure) events
    ofAddListener(smtp.events.onSMTPDelivery, this, &ofApp::onSMTPDelivery);
    ofAddListener(smtp.events.onSMTPException, this, &ofApp::onSMTPException);
}


void ofApp::draw()
{
    // Print some information about the state of the outbox.
    ofBackground(80);
    ofDrawBitmapStringHighlight("ofxSMTP: There are " + ofToString(smtp.getOutboxSize()) + " messages in your outbox.", 10,20);
}


void ofApp::keyPressed(int key)
{
    if(key == ' ')
    {
        // simple send
        smtp.send("recipient@example.com",
                  "USERNAME@gmail.com",
                  "Sent using ofxSMTP",
                  "Hello world!");
    }
}


void ofApp::onSMTPDelivery(std::shared_ptr<Poco::Net::MailMessage>& message)
{
    ofLogNotice("ofApp::onSMTPDelivery") << "Message Sent: " << message->getSubject();
}


void ofApp::onSMTPException(const ofxSMTP::ErrorArgs& evt)
{
    ofLogError("ofApp::onSMTPException") << evt.getError().displayText();

    if (evt.getMessage())
    {
        ofLogError("ofApp::onSMTPException") << evt.getMessage()->getSubject();
    }

}


void ofApp::onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogNotice("ofApp::onClientVerificationError") << std::endl << ofToString(args);

    // If you want to proceed, you must allow the user to inspect the certificate
    // and set `args.setIgnoreError(true);` if they want to continue.

    // args.setIgnoreError(true);

}

void ofApp::onSSLPrivateKeyPassphraseRequired(std::string& passphrase)
{
    // If you want to proceed, you must allow the user to input the assign the private key's
    // passphrase to the `passphrase` argument.  For example:

    passphrase = ofSystemTextBoxDialog("Enter the Private Key Passphrase", "");
}

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


#include "ofApp.h"


void ofApp::setup()
{
    // Register for SSL Context events.
    ofSSLManager::registerClientEvents(this);

    // Use the simple gmail settings (also works for any gmail based account).
    SMTP::GmailSettings settings("USERNAME@gmail.com","PASSWORD");

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


void ofApp::onSMTPDelivery(SMTP::Message::SharedPtr& message)
{
    ofLogNotice("ofApp::onSMTPDelivery") << "Message Sent: " << message->getSubject();
}


void ofApp::onSMTPException(const Poco::Exception& exc)
{
    ofLogError("ofApp::onSMTPException") << exc.displayText();
}


void ofApp::onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args)
{

    ofLogNotice("ofApp::onClientVerificationError") << std::endl << ofToString(args);

    // If you want to proceed, you must allow the user to inspect the certificate
    // and set `args.setIgnoreError(true);` if they want to continue.

    args.setIgnoreError(true);

}


void ofApp::onSSLPrivateKeyPassphraseRequired(std::string& passphrase)
{
    // If you want to proceed, you must allow the user to input the assign the private key's
    // passphrase to the `passphrase` argument.  For example:

    passphrase = ofSystemTextBoxDialog("Enter the Private Key Passphrase", "");
    
}


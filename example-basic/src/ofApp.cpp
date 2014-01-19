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


//------------------------------------------------------------------------------
void ofApp::setup()
{
    ofxSMTPGmailSettings settings("USERNAME@gmail.com","PASSWORD");
    smtp.setup(settings);
}

//------------------------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(80);
    ofDrawBitmapStringHighlight("ofxSMTP: There are " + ofToString(smtp.getNumQueued()) + " messages in your outbox.", 10,20);
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void ofApp::onDelivery(ofxMailMessage& message)
{
    ofLogNotice("ofApp::onDelivery") << "Message Sent: " << message->getSubject();
}

//------------------------------------------------------------------------------
void ofApp::onException(const Exception& exc)
{
    ofLogError("ofApp::onException") << exc.displayText();
}

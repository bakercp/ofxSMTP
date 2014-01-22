// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
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
    // Register SSL context error callbacks.
    ofSSLManager::registerClientEvents(this);

    // Set the name of the recipient for this example.
    recipientEmail = "info@christopherbaker.net";

    // Set the sender email and display name.
    senderEmail    = "Christopher Baker <info@christopherbaker.net>";

    // Load credentials and account settings from an xml file or element.
    SMTP::Settings settings = SMTP::Settings::loadFromXML("example-smtp-account-settings.xml",
                                                          "gmail");
    
    // Or use the simple gmail settings (also works for any gmail based account)
    // SMTP::GmailSettings settings("USERNAME@gmail.com","PASSWORD");

    // See SMTP::Settings for extensive configuration options.

    // Pass the settings to the client.
    smtp.setup(settings);

    // Register event callbacks for message delivery (or failure) events
    ofAddListener(smtp.events.onSMTPDelivery, this, &ofApp::onSMTPDelivery);
    ofAddListener(smtp.events.onSMTPException, this, &ofApp::onSMTPException);
}


void ofApp::exit()
{
    // It is recommended to remove event callbacks, if the SMTP::Client will
    // outlast the listener class, so it is not absolutely required in this
    // case, but it is a good practice none-the-less.
    ofRemoveListener(smtp.events.onSMTPDelivery, this, &ofApp::onSMTPDelivery);
    ofRemoveListener(smtp.events.onSMTPException, this, &ofApp::onSMTPException);
}


void ofApp::draw()
{
    // Print some information about the state of the outbox.
    ofBackground(80);
    ofDrawBitmapStringHighlight("ofxSMTP: There are " + ofToString(smtp.getOutboxSize()) + " messages in your outbox.", 10,20);
}


void ofApp::keyPressed(int key)
{
    if (key == ' ')
    {
        // simple send
        smtp.send(recipientEmail, senderEmail, "Sent using ofxSMTP", "Hello world!");

    }
    else if(key == 'a')
    {
        // You can construct complex messages using poco's MailMessage object
        // See http://pocoproject.org/docs/Poco.Net.MailMessage.html

        /// SMTP::Message::SharedPtr simply wraps Poco::Net::MailMessage.
        SMTP::Message::SharedPtr message = SMTP::Message::makeShared();

        // Encode the sender and set it.
        message->setSender(Poco::Net::MailMessage::encodeWord(senderEmail,
                                                              "UTF-8"));
        // Mark the primary recipient and add them.
        message->addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,
                                                       recipientEmail));
        
        // Encode the subject and set it.
        message->setSubject(Poco::Net::MailMessage::encodeWord("Sent using ofxSMTP",
                                                               "UTF-8"));

        // Poco::Net::MailMessage will take ownership of the *PartSource files,
        // so you don't have to worry about deleting the pointers.
        message->addContent(new Poco::Net::StringPartSource("Hello world! How about an image?"));
        
        // Poco::Net::MailMessage throws exceptions when a file is not found.
        // Thus, we need to add attachments in a try / catch block.

        try
        {
            message->addAttachment(Poco::Net::MailMessage::encodeWord("of.png","UTF-8"),
                                   new Poco::Net::FilePartSource(ofToDataPath("of.png")));
        }
        catch (const Poco::OpenFileException& exc)
        {
            ofLogError("ofApp::keyPressed") << exc.name() << " : " << exc.displayText();
        }

        // Add an additional header, just because we can.
        message->add("X-Mailer", "ofxSMTP (https://github.com/bakercp/ofxSMTP)");

        // Add the message to our outbox.
        smtp.send(message);
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

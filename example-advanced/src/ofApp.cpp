//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // Register for SSL Context events.
    ofSSLManager::registerClientEvents(this);

    // Set the name of the recipient for this example.
    recipientEmail = "info@christopherbaker.net";

    // Set the sender email and display name.
    senderEmail = "Christopher Baker <info@christopherbaker.net>";

    // Load credentials and account settings from an xml or json file.
    // auto settings = ofxSMTP::Settings::loadFromXML("example-smtp-account-settings.xml");
    auto settings = ofxSMTP::Settings::loadFromJSON("example-smtp-account-settings.json");

    // See ofxSMTP::Settings for extensive configuration options.

    // Pass the settings to the client.
    smtp.setup(settings);

    // Register event callbacks for message delivery (or failure) events
    smtpDeliveryListener = smtp.events.onSMTPDelivery.newListener(this, &ofApp::onSMTPDelivery);
    smtpExceptionListener = smtp.events.onSMTPException.newListener(this, &ofApp::onSMTPException);
}


void ofApp::draw()
{
    // Print some information about the state of the outbox.
    ofBackground(80);
    
    std::stringstream ss;
    ss << "         Press <SPACEBAR> to Send Text" << std::endl;
    ss << "           Press <a> to Send an Image" << std::endl;
    ss << "ofxSMTP: There are " + ofToString(smtp.getOutboxSize()) + " messages in your outbox.";
    ofDrawBitmapStringHighlight(ss.str(), 10, 20);
}


void ofApp::keyPressed(int key)
{
    if (key == ' ') // Press spacebar for a simple send.
    {
        // Send a simple short message.
        smtp.send(recipientEmail,             // Recipient email.
                  senderEmail,                // Sender email.
                  "I'm trying out ofxSMTP!",  // Subject line.
                  "It works!");               // Message body.
    }
    else if(key == 'a') // Press 'a' for an advanced send with attachment.
    {
        // You can construct complex messages using POCO's MailMessage object.
        // See http://pocoproject.org/docs/Poco.Net.MailMessage.html

        auto message = std::make_shared<Poco::Net::MailMessage>();

        // Encode the sender and set it.
        message->setSender(Poco::Net::MailMessage::encodeWord(senderEmail,
                                                              "UTF-8"));
        // Mark the primary recipient and add them.
        message->addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,
                                                       recipientEmail));
        
        // Encode the subject and set it.
        message->setSubject(Poco::Net::MailMessage::encodeWord("I'm sending you an image using ofxSMTP!",
                                                               "UTF-8"));

        // Poco::Net::MailMessage will take ownership of the *PartSource files,
        // so you don't have to worry about deleting the pointers.
        message->addContent(new Poco::Net::StringPartSource("Hello world! How about an image?"));
        
        // Poco::Net::MailMessage throws exceptions when a file is not found.
        // Thus, we need to add attachments in a try / catch block.

        try
        {
            message->addAttachment(Poco::Net::MailMessage::encodeWord("of.png","UTF-8"),
                                   new Poco::Net::FilePartSource(ofToDataPath("of.png", true)));
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

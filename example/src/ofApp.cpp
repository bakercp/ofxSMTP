#include "ofApp.h"

//------------------------------------------------------------------------------
void ofApp::setup() {

    recipientEmail = "TO@example.com";
    senderEmail    = "Your Name <FROM@example.com>"; // you can also include your display name

    // you can load settings from an xml file or element
    ofxSMTPSettings settings = ofxSMTPSettings::loadFromXML("example-smtp-account-settings.xml","gmail");
    
    // Or use the simple gmail settings (also works for any gmail based account)
    // ofxSMTPGmailSettings settings("USERNAME@gmail.com","PASSWORD");
    
    // See ofxSMTPSettings for extensive configuration options.
    
    smtp.setup(settings);

    // you can also register for message delivery (or failure) events
    ofAddListener(smtp.events.onDelivery, this, &ofApp::onDelivery);
    ofAddListener(smtp.events.onException, this, &ofApp::onException);
    
}

//------------------------------------------------------------------------------
void ofApp::exit() {
    ofRemoveListener(smtp.events.onDelivery, this, &ofApp::onDelivery);
    ofRemoveListener(smtp.events.onException, this, &ofApp::onException);
}

//------------------------------------------------------------------------------
void ofApp::draw(){
    ofBackground(80);
    ofDrawBitmapStringHighlight("ofxSMTP: There are " + ofToString(smtp.getNumQueued()) + " messages in your outbox.", 10,20);
}

//------------------------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(key == ' ') {
        // simple send
        smtp.send(recipientEmail, senderEmail, "Sent using ofxSMTP", "Hello world!");
    } else if(key == 'a') {
        // you can construct complex messages using poco's MailMessage object
        // See http://pocoproject.org/docs/Poco.Net.MailMessage.html
        ofxMailMessage message = ofxMailMessage(new MailMessage());
        message->setSender(MailMessage::encodeWord(senderEmail, "UTF-8"));
        message->addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, recipientEmail));
        message->setSubject(MailMessage::encodeWord("Sent using ofxSMTP", "UTF-8"));
        
        // MailMessage will take ownership of the *PartSource files,
        // so you don't have to worry about deleting the pointers.
        message->addContent(new StringPartSource("Hello world! How about an image?"));
        
        // Poco::MailMessage throws exceptions when a file is not present,
        // so we need to catch it.
        try {
            message->addAttachment(MailMessage::encodeWord("of.png","UTF-8"),
                                   new FilePartSource(ofToDataPath("of.png")));
        } catch (const OpenFileException& exc) {
            ofLogError("ofApp::keyPressed") << exc.name() << " : " << exc.displayText();
        }

        // send it!
        smtp.send(message);
    }
}

//------------------------------------------------------------------------------
void ofApp::onDelivery(ofxMailMessage& message) {
    ofLogNotice("ofApp::onDelivery") << "Message Sent: " << message->getSubject();
}

//------------------------------------------------------------------------------
void ofApp::onException(Exception& exc) {
    ofLogError("ofApp::onException") << exc.displayText();
}

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    recipientEmail = "TO@example.com";
    senderEmail    = "Your Name <FROM@example.com>"; // you can also include your display name

    // you can load settings from an xml file or element
    ofxSMTPSettings settings = ofxSMTPSettings::loadFromXML("smtp-account-settings.xml","gmail");
    
    // Or use the simple gmail settings:
    // ofxSMTPGmailSettings settings("USERNAME@gmail.com","PASSWORD");
    
    // See ofxSMTPSettings for extensive configuration options.
    
    smtp.setup(settings);

    // you can also register for message delivery (or failure) events
    ofAddListener(smtp.events.status, this, &testApp::messageStatus);
    
}

void testApp::exit() {
    ofRemoveListener(smtp.events.status, this, &testApp::messageStatus);
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(80);
    ofDrawBitmapStringHighlight("ofxSMTP: There are " + ofToString(smtp.getNumQueued()) + " messages in your outbox.", 10,20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
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
        message->addContent(new StringPartSource("Hello world! How about an image?"));
        
        try {
            message->addAttachment(MailMessage::encodeWord("of.png","UTF-8"),
                                   new FilePartSource(ofToDataPath("of.png")));
        } catch (const OpenFileException& exc) {
            ofLogError("testApp::keyPressed") << exc.name() << " : " << exc.displayText();
        }

        // send it!
        smtp.send(message);
    }
}

//--------------------------------------------------------------
void testApp::messageStatus(ofxSMTPEventArgs& evt) {
    ofLogNotice("testApp::messageStatus") << evt.message->getSubject() << " delivered " << (evt.delivered ? "SUCCESSFULLY" : "UNSUCCESSFULLY");
}

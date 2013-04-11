#pragma once

#include "Poco/Exception.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/StringPartSource.h"

#include "ofMain.h"

#include "ofxSMTP.h"

using Poco::OpenFileException;
using Poco::Net::FilePartSource;
using Poco::Net::StringPartSource;

class ofApp : public ofBaseApp {
public:
    void setup();
    void draw();
    void exit();
    
    void keyPressed(int key);

    void onDelivery(ofxMailMessage& message);
    void onException(const Exception& exc);

    string  recipientEmail;
    string  senderEmail;
    ofxSMTP smtp;
};

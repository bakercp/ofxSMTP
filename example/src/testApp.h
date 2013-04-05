#pragma once

#include "Poco/Exception.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/StringPartSource.h"

#include "ofMain.h"

#include "ofxSMTP.h"

using Poco::OpenFileException;
using Poco::Net::FilePartSource;
using Poco::Net::StringPartSource;

class testApp : public ofBaseApp{
public:
    void setup();
    void draw();
    void exit();
    
    void keyPressed(int key);

    void messageStatus(ofxSMTPEventArgs& evt);

    string  recipientEmail;
    string  senderEmail;
    ofxSMTP smtp;
};

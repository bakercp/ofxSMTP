//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofApp.h"


int main()
{
    ofSetupOpenGL(380, 66, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}


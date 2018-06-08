//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Exception.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/DateTimeFormatter.h"
#include "ofSSLManager.h"
#include "ofx/SMTP/Events.h"
#include "ofx/SMTP/Client.h"
#include "ofx/SMTP/Credentials.h"
#include "ofx/SMTP/GmailSettings.h"
#include "ofx/SMTP/Settings.h"


namespace ofxSMTP = ofx::SMTP;

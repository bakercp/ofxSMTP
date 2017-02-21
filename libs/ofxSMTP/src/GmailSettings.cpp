//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//



#include "ofx/SMTP/GmailSettings.h"



namespace ofx {
namespace SMTP {


const std::string GmailSettings::DEFAULT_GMAIL_HOST = "smtp.gmail.com";


GmailSettings::GmailSettings(const std::string& username,
                             const std::string& password):
    STARTTLSSettings(DEFAULT_GMAIL_HOST,
                     DEFAULT_GMAIL_PORT,
                     Credentials(username,
                                 password,
                                 Poco::Net::SMTPClientSession::AUTH_LOGIN))
{
}


GmailSettings::~GmailSettings()
{
}


} } // namespace ofx::SMTP


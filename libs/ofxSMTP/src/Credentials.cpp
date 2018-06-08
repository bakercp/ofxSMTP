//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/SMTP/Credentials.h"


namespace ofx {
namespace SMTP {



Credentials::Credentials(const std::string& username,
                         const std::string& password,
                         const LoginMethod& loginMethod):
    _username(username),
    _password(password),
    _loginMethod(loginMethod)
{
}


Credentials::~Credentials()
{
}


std::string Credentials::username() const
{
    return _username;
}

    
std::string Credentials::getUsername() const
{
    return username();
}

    
std::string Credentials::password() const
{
    return _password;
}


std::string Credentials::getPassword() const
{
    return password();
}

    
Credentials::LoginMethod Credentials::loginMethod() const
{
    return _loginMethod;
}


Credentials::LoginMethod Credentials::getLoginMethod() const
{
    return loginMethod();
}
    
    
Credentials::LoginMethod Credentials::from_string(const std::string& method)
{
    if (method == "AUTH_NONE")
        return Poco::Net::SMTPClientSession::AUTH_NONE;
    else if (method == "AUTH_LOGIN")
        return Poco::Net::SMTPClientSession::AUTH_LOGIN;
    else if (method == "AUTH_CRAM_MD5")
        return Poco::Net::SMTPClientSession::AUTH_CRAM_MD5;
    else if (method == "AUTH_CRAM_SHA1")
        return Poco::Net::SMTPClientSession::AUTH_CRAM_SHA1;
    else if (method == "AUTH_PLAIN")
        return Poco::Net::SMTPClientSession::AUTH_PLAIN;
    else if (method == "AUTH_XOAUTH2")
        return Poco::Net::SMTPClientSession::AUTH_XOAUTH2;
    
    ofLogError("Credentials::from_string") << "Unknown method: " << method;
    return Poco::Net::SMTPClientSession::AUTH_NONE;
}
    
    
std::string Credentials::to_string(const LoginMethod& method)
{
    switch (method)
    {
        case Poco::Net::SMTPClientSession::AUTH_NONE:
            return "AUTH_NONE";
        case Poco::Net::SMTPClientSession::AUTH_LOGIN:
            return "AUTH_LOGIN";
        case Poco::Net::SMTPClientSession::AUTH_CRAM_MD5:
            return "AUTH_CRAM_MD5";
        case Poco::Net::SMTPClientSession::AUTH_CRAM_SHA1:
            return "AUTH_CRAM_SHA1";
        case Poco::Net::SMTPClientSession::AUTH_PLAIN:
            return "AUTH_PLAIN";
        case Poco::Net::SMTPClientSession::AUTH_XOAUTH2:
            return "AUTH_XOAUTH2";
    }
    
    ofLogError("Credentials::to_string") << "Unknown method: " << method;
    return "AUTH_NONE";
}


} } // namespace ofx::SMTP

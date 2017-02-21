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


std::string Credentials::getUsername() const
{
    return _username;
}


std::string Credentials::getPassword() const
{
    return _password;
}


Credentials::LoginMethod Credentials::getLoginMethod() const
{
    return _loginMethod;
}

    
} } // namespace ofx::SMTP

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
    
    
} } // namespace ofx::SMTP

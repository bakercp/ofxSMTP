//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "Poco/Net/SMTPClientSession.h"
#include "ofConstants.h"
#include "ofJson.h"


namespace ofx {
namespace SMTP {


/// \brief An class to represent SMTP Credentials.
class Credentials
{
public:
    /// \brief A typedef for a LoginMethod.
    typedef Poco::Net::SMTPClientSession::LoginMethod LoginMethod;

    /// \brief Create Credentials.
    /// \param username The account username.
    /// \param password The account password.
    /// \param loginMethod The login security method.
    Credentials(const std::string& username = "",
                const std::string& password = "",
                const LoginMethod& loginMethod = Poco::Net::SMTPClientSession::AUTH_NONE);

    /// \brief Destroy the Credentials.
    virtual ~Credentials();

    /// \returns The account username.
    std::string username() const;
    OF_DEPRECATED_MSG("Use username().", std::string getUsername() const);

    /// \returns The account password.
    std::string password() const;
    OF_DEPRECATED_MSG("Use password().", std::string getPassword() const);

    /// \returns The login security method.
    LoginMethod loginMethod() const;
    OF_DEPRECATED_MSG("Use loginMethod().", LoginMethod getLoginMethod() const);

    /// \brief Create Credentials from json.
    /// \param json The json to parse.
    /// \returns credentials.
    static Credentials fromJson(const ofJson& json);
    
private:
    /// \brief Convert a string to a LoginMethod.
    /// \param method The method to convert.
    /// \returns the LoginMethod.
    static LoginMethod from_string(const std::string& method);

    /// \brief Convert a LoginMethod to a std::string.
    /// \param method The method to convert.
    /// \returns the converted string.
    static std::string to_string(const LoginMethod& method);
    
    /// \brief The account username.
    std::string _username;

    /// \brief The account password.
    std::string _password;

    /// \brief the Account login security method.
    LoginMethod _loginMethod;

    friend class Settings;
    
};


} } // namespace ofx::SMTP

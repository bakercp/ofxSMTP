//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "Poco/Net/SMTPClientSession.h"


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
    std::string getUsername() const;

    /// \returns The account password.
    std::string getPassword() const;

    /// \returns The login security method.
    LoginMethod getLoginMethod() const;

private:
    /// \brief The account username.
    std::string _username;

    /// \brief The account password.
    std::string _password;

    /// \brief the Account login security method.
    LoginMethod _loginMethod;

};


} } // namespace ofx::SMTP

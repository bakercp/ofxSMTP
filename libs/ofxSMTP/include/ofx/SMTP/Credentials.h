// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <string>
#include "Poco/Net/SMTPClientSession.h"


namespace ofx {
namespace SMTP {


class Credentials
    /// \brief An class to represent SMTP Credentials.
{
public:
    typedef Poco::Net::SMTPClientSession::LoginMethod LoginMethod;
        ///< \brief A typedef for a LoginMethod.

    Credentials(const std::string& username = "",
                const std::string& password = "",
                const LoginMethod& loginMethod = Poco::Net::SMTPClientSession::AUTH_NONE);
        ///< \brief Create Credentials.
        ///< \param username The account username.
        ///< \param password The account password.
        ///< \param loginMethod The login security method.

    virtual ~Credentials();
        ///< \brief Destroy the Credentials.
    
    std::string getUsername() const;
        ///< \returns The account username.

    std::string getPassword() const;
        ///< \returns The account password.

    LoginMethod getLoginMethod() const;
        ///< \returns The login security method.

private:
    std::string _username;
        ///< \brief The account username.

    std::string _password;
        ///< \brief The account password.

    LoginMethod _loginMethod;
        ///< \brief the Account login security method.
    
};


} } // namespace ofx::SMTP

//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <memory>
#include "Poco/Net/MailMessage.h"


namespace ofx {
namespace SMTP {


/// \brief A wrapper for Poco::Net::MailMessage.
class Message
{
public:
    /// \brief A typedef to a SharedPtr.
    /// \deprecated
    //typedef std::shared_ptr<Poco::Net::MailMessage> SharedPtr;

    /// \brief Make a shared pointer;
    /// \todo Replace with std::make_shared with C++11.
    /// \deprecated
    static std::shared_ptr<Poco::Net::MailMessage> makeShared()
    {
        return std::make_shared<Poco::Net::MailMessage>();
    }
    
private:
    Message();
    Message(const Message&);
	Message& operator = (const Message&);

};

} } // namespace ofx::SMTP

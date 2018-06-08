//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/SMTP/Events.h"


namespace ofx {
namespace SMTP {


ErrorArgs::ErrorArgs(const Poco::Exception& error): _error(error)
{
}


ErrorArgs::ErrorArgs(const Poco::Exception& error,
                     std::shared_ptr<Poco::Net::MailMessage> message):
    _error(error),
    _message(message)
{
}


ErrorArgs::~ErrorArgs()
{
}


const Poco::Exception& ErrorArgs::error() const
{
    return _error;
}

    
const Poco::Exception& ErrorArgs::getError() const
{
    return error();
}

    
std::shared_ptr<Poco::Net::MailMessage> ErrorArgs::message() const
{
    return _message;
}

    
std::shared_ptr<Poco::Net::MailMessage> ErrorArgs::getMessage() const
{
    return message();
}


} } // namespace ofx::SMTP

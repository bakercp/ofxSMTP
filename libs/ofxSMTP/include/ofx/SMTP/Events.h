//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Exception.h"
#include "ofEvents.h"


namespace ofx {
namespace SMTP {


class ErrorArgs
{
public:
    ErrorArgs(const Poco::Exception& error):
        _error(error)
    {
    }


    ErrorArgs(const Poco::Exception& error,
              std::shared_ptr<Poco::Net::MailMessage> message):
        _error(error),
        _message(message)
    {
    }


    ~ErrorArgs()
    {
    }


    /// \returns The error message.
    const Poco::Exception& getError() const
    {
        return _error;
    }


    /// \returns A valid pointer to the associated message, iff available.
    std::shared_ptr<Poco::Net::MailMessage> getMessage() const
    {
        return _message;
    }

protected:
    const Poco::Exception _error;
    std::shared_ptr<Poco::Net::MailMessage> _message;
    
};


/// \brief A collection of SMTP events.
/// \todo Add progress once Poco supports it
/// http://pocoproject.org/forum/viewtopic.php?f=12&t=5655&p=9788&hilit=smtp#p9788
class ClientEvents
{
public:
    /// \brief This event is triggered upon successful message delivery.
    ofEvent<std::shared_ptr<Poco::Net::MailMessage>> onSMTPDelivery;

    /// \brief This message is triggered upon client error.
    ofEvent<const ErrorArgs> onSMTPException;
    
};


} } // namespace ofx::SMTP

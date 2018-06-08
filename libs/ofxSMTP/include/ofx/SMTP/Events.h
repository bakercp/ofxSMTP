//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Exception.h"
#include "Poco/Net/MailMessage.h"
#include "ofEvents.h"


namespace ofx {
namespace SMTP {


/// \brief A class used for event callbacks.
class ErrorArgs
{
public:
    /// \brief Create the ErrorArgs.
    /// \param error The exception.
    ErrorArgs(const Poco::Exception& error);

    /// \brief Create the ErrorArgs.
    /// \param error The exception.
    /// \param message The message.
    ErrorArgs(const Poco::Exception& error,
              std::shared_ptr<Poco::Net::MailMessage> message);

    /// \brief Destroy the ErrorArgs.
    ~ErrorArgs();

    /// \returns The error message.
    const Poco::Exception& error() const;
    OF_DEPRECATED_MSG("Use error().", const Poco::Exception& getError() const);

    /// \returns A pointer to the associated message or nullptr if unset.
    std::shared_ptr<Poco::Net::MailMessage> message() const;
    OF_DEPRECATED_MSG("Use message().", std::shared_ptr<Poco::Net::MailMessage> getMessage() const);

protected:
    /// \brief The error.
    const Poco::Exception _error;
    
    /// \brief The associated message.
    std::shared_ptr<Poco::Net::MailMessage> _message = nullptr;
    
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

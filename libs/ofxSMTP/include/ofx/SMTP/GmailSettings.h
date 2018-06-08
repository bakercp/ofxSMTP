//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/SMTP/Settings.h"


namespace ofx {
namespace SMTP {


/// \brief A convenience settings file for Gmail.
///
/// Configures credentials for a Gmail / Gsuite account using STARTTLS and port
/// 587 and the smtp.gmail.com host.
///
/// Gmail also works on port 465 with SSL/TLS encryption. Since port 587 is
/// often blocked users might prefer to use port 465 with SSL/TLS encryption.
/// To use alternative settings for Gmail, manually configure your client
/// using ofx::SMTP::Settings e.g:
///
///     ofxSMTP::Credentials credentials("USERNAME@gmail.com",
///                                      "PASSWORD",
///                                      Poco::Net::SMTPClientSession::AUTH_LOGIN);
///
///     ofxSMTP::Settings settings("smtp.gmail.com", 465, credentials,
///                                ofxSMTP::Settings::EncryptionType::SSLTLS);
///
class GmailSettings: public STARTTLSSettings
{
public:
    /// \brief Create a GmailSettings object.
    /// \param username The account username.
    /// \param password The account password.
    GmailSettings(const std::string& username,
                  const std::string& password);

    /// \brief Destroy the GmailSettings.
    virtual ~GmailSettings();

    /// \brief The default Gmail STARTTLS host.
    static const std::string DEFAULT_GMAIL_HOST;

    enum
    {
        /// \brief The default Gmail STARTTLS port.
        DEFAULT_GMAIL_PORT = DEFAULT_SMTP_STARTTLS_PORT
    };
};


} } // namespace ofx::SMTP

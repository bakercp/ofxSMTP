//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "Poco/Timespan.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "ofx/SMTP/Credentials.h"
#include "ofConstants.h"
#include "ofJson.h"


namespace ofx {
namespace SMTP {


/// \brief A base class representing SMTP Client settings.
class Settings
{
public:
    /// \brief The encryption type.
    enum EncryptionType
    {
        /// \brief Use no encyption.
        NONE,
        /// \brief Use an SSLTLS strategy.
        SSLTLS,
        /// \brief Use a STARTTLS connection.
        STARTTLS
    };

    /// \brief Create SMTP Settings.
    /// \param host The SMTP server host.
    /// \param port The SMTP server port.
    /// \param credentials The SMTP Credentials settings.
    /// \param encryption The SMTP encryption settings.
    /// \param timeout The client timeout.
    /// \param messageSendDelay The delay between sending messages.
    Settings(const std::string& host = "",
             uint16_t port = DEFAULT_SMTP_PORT,
             Credentials credentials = Credentials(),
             EncryptionType encryptionType = NONE,
             Poco::Timespan timeout = DEFAULT_TIMEOUT,
             Poco::Timespan messageSendDelay = DEFAULT_MESSAGE_SEND_DELAY);

    /// \brief Destroy the SMTP Settings.
    virtual ~Settings();

    /// \returns The SMTP Server host.
    std::string host() const;
    OF_DEPRECATED_MSG("Use host().", std::string getHost() const);
    
    /// \returns The SMTP Server port.
    uint16_t port() const;
    OF_DEPRECATED_MSG("Use port().", uint16_t getPort() const);

    /// \returns The SMTP Credential settings.
    Credentials credentials() const;
    OF_DEPRECATED_MSG("Use credentials().", Credentials getCredentials() const);

    /// \returns The SMTP Encryption settings.
    EncryptionType encryptionType() const;
    OF_DEPRECATED_MSG("Use encryptionType().", EncryptionType getEncryptionType() const);

    /// \returns The client timeout.
    Poco::Timespan timeout() const;
    OF_DEPRECATED_MSG("Use timeout().", Poco::Timespan getTimeout() const);

    /// \returns The delay between sending message.
    Poco::Timespan messageSendDelay() const;
    OF_DEPRECATED_MSG("Use messageSendDelay().", Poco::Timespan getMessageSendDelay() const);

    /// \brief Load settings from JSON.
    /// \param json The JSON.
    /// \returns Settings loaded from a file.
    static Settings fromJSON(const ofJson& json);
    
    /// \brief Load settings from an XML file.
    /// \param filename The file name for the XML file.
    /// \returns Settings loaded from a file.
    static Settings loadFromXML(const std::string& filename);

    /// \brief Load settings from an JSON file.
    /// \param filename The file name for the JSON file.
    /// \returns Settings loaded from a file.
    static Settings loadFromJSON(const std::string& filename);

    /// \brief Load the settings from a Poco::Util::AbstractConfiguration.
    /// \param config the Poco::Util::AbstractConfiguration.
    /// \returns Settings loaded from a file.
    /// \throws Poco::NotFoundException and others.
    static Settings load(const Poco::Util::AbstractConfiguration& config);

    /// \brief The default client timeout.
    static const Poco::Timespan DEFAULT_TIMEOUT;

    /// \brief The delay between sending messages.
    static const Poco::Timespan DEFAULT_MESSAGE_SEND_DELAY;

    enum
    {
        /// \brief Default SMTP Port.
        DEFAULT_SMTP_PORT = 25,
        /// \brief Default Secure SMTP Port.
        /// \note Gmail uses port 465.
        DEFAULT_SMTP_SSL_PORT = 425,
        /// \brief Default secure STARTTLS SMTP Port.
        DEFAULT_SMTP_STARTTLS_PORT = 587
    };

private:
    /// \brief Convert a string to an Settings::EncryptionType.
    /// \param method The method to convert.
    /// \returns the encryption type.
    static Settings::EncryptionType from_string(const std::string& method);
    
    /// \brief Convert a Settings::EncryptionType to a std::string.
    /// \param method The method to convert.
    /// \returns the converted string.
    static std::string to_string(const Settings::EncryptionType& method);

    /// \brief SMTP server host.
    std::string _host;

    /// \brief SMTP server port.
    uint16_t _port;

    /// \brief Credentials settings.
    Credentials _credentials;

    /// \brief Encryption settings.
    EncryptionType  _encryptionType;

    /// \brief Client timeout.
    Poco::Timespan _timeout;

    /// \brief The delay between sending messages.
    Poco::Timespan _messageSendDelay;

};


/// \brief A convenience class representing Secure SMTP Client settings.
class SSLTLSSettings: public Settings
{
public:
    /// \brief Create a SSLTLSSettings object.
    /// \param host The SMTP Server host.
    /// \param port The SMTP Server port.
    /// \param credentials The SMTP Credentials settings.
    SSLTLSSettings(const std::string& host,
                   uint16_t port = DEFAULT_SMTP_SSL_PORT,
                   Credentials credentials = Credentials());

    /// \brief Destroy the SSLTLSSettings.
    virtual ~SSLTLSSettings();

};


/// \brief A convenience class representing STARTTLS SMTP Client settings.
class STARTTLSSettings: public Settings
{
public:
    /// \brief Create a STARTTLSSettings object.
    /// \param host The SMTP Server host.
    /// \param port The SMTP Server port.
    /// \param credentials The SMTP Credentials settings.
    STARTTLSSettings(const std::string& host,
                     uint16_t port = DEFAULT_SMTP_STARTTLS_PORT,
                     Credentials credentials = Credentials());

    /// \brief Destroy the SSLTLSSettings.
    virtual ~STARTTLSSettings();
    
};


} } // namespace ofx::SMTP

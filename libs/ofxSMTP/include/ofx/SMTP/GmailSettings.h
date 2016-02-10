// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/SMTP/Settings.h"


namespace ofx {
namespace SMTP {


/// \brief A convenience settings file for Gmail.
/// \details Gmail also works on port 465 with SSL/TLS encryption. Since port
///          587 is often blocked, the user might prefer to use port 465 with
///          SSL/TLS encryption. To use alternative settings, manually configure
//           your client settings.
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

    /// \brief The default Gmail host.
    static const std::string DEFAULT_GMAIL_HOST;

    enum
    {
        /// \brief The default Gmail port.
        DEFAULT_GMAIL_PORT = DEFAULT_SMTP_STARTTLS_PORT
    };
};


} } // namespace ofx::SMTP

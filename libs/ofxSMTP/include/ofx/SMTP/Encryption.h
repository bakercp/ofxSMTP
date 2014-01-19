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


#include "Poco/Net/Context.h"
#include "Poco/Net/MailMessage.h"
#include "ofUtils.h"
#include "ofx/SMTP/Types.h"


namespace ofx {
namespace SMTP {


class Encryption
    ///< \brief Represents parameters used for SSL encryption.
{
public:
    typedef Poco::Net::Context::VerificationMode VerificationMode;
        ///< \brief A typedef for VerificationMode.

    enum Type
        ///< \brief The encryption type.
    {
        NONE,   ///< \brief Use no encyption.
        SSLTLS, ///< \brief Use an SSLTLS strategy.
        STARTTLS ///< \brief Use a STARTTLS connection.
    };

    Encryption(Type type, Poco::Net::Context::Ptr context);
        ///< \brief Create an Encryption object.
        ///< \param type The encryption Type.
        ///< \param context The encryption context.

    Encryption(Type type = NONE,
               const std::string& caLocation = "cacert.pem",
               VerificationMode verificationMode = Poco::Net::Context::VERIFY_RELAXED);
        ///< \brief Create an Encryption object.
        ///< \param type The encryption Type.
        ///< \param caLocation The location of the CA .pem file used to verify SSL certs.
        ///< \param verificationMode The SSL verification mode.

    virtual ~Encryption();
        ///< \brief Destroy the Encruption.

    Type getType() const;
        ///< \brief Get the type of encryption associated with this object.

    Poco::Net::Context::Ptr getContext() const;
        ///< \brief Get the Poco::Net::Context associated with this object.

protected:
    Type _type; ///< \brief The encryption Type
    Poco::Net::Context::Ptr _context; ///< \brief The encryption context.
    
};


} } // namespace ofx::SMTP

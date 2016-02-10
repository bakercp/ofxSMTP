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
    typedef std::shared_ptr<Poco::Net::MailMessage> SharedPtr;

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

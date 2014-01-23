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


#include "Poco/Exception.h"
#include "ofEvents.h"
#include "ofx/SMTP/Types.h"


namespace ofx {
namespace SMTP {


class ClientEvents
    /// \brief A collection of SMTP events.
    /// \todo Add progress once Poco supports it
    /// http://pocoproject.org/forum/viewtopic.php?f=12&t=5655&p=9788&hilit=smtp#p9788
{
public:
    ofEvent<Message::SharedPtr> onSMTPDelivery;
        ///< \brief This event is triggered upon successful message delivery.

    ofEvent<const Poco::Exception> onSMTPException;
        ///< \brief This message is triggered upon client error.
};


} } // namespace ofx::SMTP

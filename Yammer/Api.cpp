/**
* Yawner
*
* Copyright (C) 2011 Henrik Hedelund (henke.hedelund@gmail.com)
*
* This file is part of the Qt based Yammer Client "Yawner".
*
* Yawner is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Yawner is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Yawner. If not, see <http://www.gnu.org/licenses/>.
*
* @category Yammer
* @package Yammer
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/lgpl.html GNU GPL
* @link https://github.com/henkelund/Yawner
*/

#include "Api.h"
#include <QNetworkReply>
#include <QUrl>
#include "../OAuth/Consumer.h"
#include "../OAuth/SignatureMethod/Plaintext.h"
#include "../OAuth/Request.h"
#include "../OAuth/Token.h"

namespace Yammer {

    Api::Api(OAuthNS::Consumer consumer, QObject *parent) :
        QObject(parent), _consumer(consumer)
    {
    }

    void Api::getRequestToken()
    {
        //create request
        OAuthNS::Request *request = OAuthNS::Request::fromConsumer(
            _consumer,
            QString("https://www.yammer.com/oauth/request_token")
        );

        OAuthNS::SignatureMethodNS::Plaintext sm;
        request->signRequest(&sm, _consumer);

        // OAuthNS::Response will take ownership of the reply object
        QNetworkReply *reply = request->exec();
        reply->setProperty("is_token_request", QVariant(true));

        // register callback
        QObject::connect(
            request,
            SIGNAL(responseRecieved(OAuthNS::Response*)),
            SLOT(responseRecieved(OAuthNS::Response*))
        );
    }

    void Api::call(const char *method)
    {

    }

    void Api::responseRecieved(OAuthNS::Response *response)
    {
        if (response->getReplyObject()->property("is_token_request").isValid()) {

            emit requestTokenRecieved(
                response->property("oauth_token").toString(),
                response->property("oauth_token_secret").toString()
            );
        }
    }
}

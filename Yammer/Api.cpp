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
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link https://github.com/henkelund/Yawner
*/

#include "Api.h"
#include <QNetworkReply>
#include <QUrl>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include "../OAuth/Consumer.h"
#include "../OAuth/SignatureMethod/Plaintext.h"
#include "../OAuth/Request.h"
#include "../OAuth/Token.h"
#include "../Yawner.h"

namespace YammerNS {

    Api::Api(OAuthNS::Consumer consumer, QObject *parent) :
        QObject(parent), _consumer(consumer), _accessToken()
    {
    }

    void Api::setAccessToken(OAuthNS::Token token)
    {
        _accessToken = token;
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
        reply->setProperty("is_request_token_request", QVariant(true));

        // register callback
        QObject::connect(
            request,
            SIGNAL(responseRecieved(OAuthNS::Response*)),
            SLOT(responseRecieved(OAuthNS::Response*))
        );
    }

    void Api::getAccessToken(OAuthNS::Token requestToken, QString verifyer)
    {
        //create request
        OAuthNS::Request *request = OAuthNS::Request::fromConsumerAndToken(
            _consumer,
            requestToken,
            QString("https://www.yammer.com/oauth/access_token")
        );

        request->setParameter(QString("oauth_verifier"), verifyer);

        OAuthNS::SignatureMethodNS::Plaintext sm;
        request->signRequest(&sm, _consumer, requestToken);

        // OAuthNS::Response will take ownership of the reply object
        QNetworkReply *reply = request->exec();
        reply->setProperty("is_access_token_request", QVariant(true));

        // register callback
        QObject::connect(
            request,
            SIGNAL(responseRecieved(OAuthNS::Response*)),
            SLOT(responseRecieved(OAuthNS::Response*))
        );
    }

    OAuthNS::Request* Api::get(
        QUrl url,
        QObject* recieverObject,
        const char* recieverMethod)
    {
        OAuthNS::Request *request = new OAuthNS::Request(OAuthNS::Request::GET, url.toString());
        // OAuthNS::Response will take ownership of the reply object
        QNetworkReply *reply = request->exec();
        connect(
            reply,
            SIGNAL(downloadProgress(qint64,qint64)),
            request,
            SLOT(downloadProgress(qint64,qint64))
        );
        // register callback
        connect(
            request,
            SIGNAL(responseRecieved(OAuthNS::Response*)),
            recieverObject,
            recieverMethod
        );

        return request;
    }

    OAuthNS::Request* Api::get(
        QString resource,
        QObject *recieverObject,
        const char* recieverMethod,
        QMap<QString, QString> params)
    {
        //create request
        OAuthNS::Request *request = OAuthNS::Request::fromConsumerAndToken(
            _consumer,
            _accessToken,
            QString("https://www.yammer.com/api/v1/%1.json").arg(resource)
        );

        QMapIterator<QString, QString> paramIt(params);
        while (paramIt.hasNext()) {
            paramIt.next();
            request->getUrl()->addQueryItem(paramIt.key(), paramIt.value());
        }

        OAuthNS::SignatureMethodNS::Plaintext sm;
        request->signRequest(&sm, _consumer, _accessToken);
        // OAuthNS::Response will take ownership of the reply object
        QNetworkReply *reply = request->exec();
        connect(
            reply,
            SIGNAL(downloadProgress(qint64,qint64)),
            request,
            SLOT(downloadProgress(qint64,qint64))
        );
        // register callback
        connect(
            request,
            SIGNAL(responseRecieved(OAuthNS::Response*)),
            recieverObject,
            recieverMethod
        );

        return request;
    }

    void Api::responseRecieved(OAuthNS::Response *response)
    {
        if (response->getReplyObject()->property("is_request_token_request").isValid()) {

            emit requestTokenRecieved(
                OAuthNS::Token(
                    response->property("oauth_token").toString(),
                    response->property("oauth_token_secret").toString()
                )
            );
        }
        else if (response->getReplyObject()->property("is_access_token_request").isValid()) {

            emit accessTokenRecieved(
                OAuthNS::Token(
                    response->property("oauth_token").toString(),
                    response->property("oauth_token_secret").toString()
                )
            );
        }
        else if (response->getReplyObject()->property("is_call_request").isValid()) {

            //qDebug(response->getRawContent().toStdString().c_str());
        }

        response->deleteLater();
    }
}

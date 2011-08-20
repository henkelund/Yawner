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
#include <QTimer>
#include "../OAuth/Consumer.h"
#include "../OAuth/SignatureMethod/Plaintext.h"
#include "../OAuth/Request.h"
#include "../OAuth/Token.h"
#include "../Yawner.h"

namespace YammerNS {

    Api::Api(OAuthNS::Consumer consumer, QObject *parent) :
        QObject(parent), _consumer(consumer), _accessToken(), _realtimeSettings()
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

    bool Api::realtimeSubscribe(QMap<QString, QVariant> settings)
    {
        if (_realtimeSettings.isEmpty()) {
            _realtimeSettings = settings;
            _realtimeRequest();
            return true;
        }
        return false;
    }

    OAuthNS::Request* Api::_realtimeRequest()
    {
        int id = 0;

        QString url = _realtimeSettings.value("uri").toString();
        QString clientId =
                _realtimeSettings.value("clientId").canConvert(QVariant::String) ?
                    _realtimeSettings.value("clientId").toString() : QString();
        QString channelId =
                _realtimeSettings.value("channel_id").canConvert(QVariant::String) ?
                    _realtimeSettings.value("channel_id").toString() : QString();
        bool subscribed = _realtimeSettings.contains("subscriptions") &&
                _realtimeSettings.value("subscriptions").canConvert(QVariant::List);
        QList<QVariant> body;

        if (_realtimeSettings.contains("id") && _realtimeSettings.value("id").canConvert(QVariant::Int)) {
            bool ok = false;
            id = _realtimeSettings.value("id").toInt(&ok);
            if (!ok) {
                id = 0;
            }
        }

        if (0 == id || clientId.isEmpty()) {
            url.append("handshake");

            QMap<QString, QVariant> ext;
            ext.insert("token", _realtimeSettings.value("authentication_token").toString());
            QMap<QString, QVariant> bodyObject;
            bodyObject.insert("ext", ext);
            bodyObject.insert("version", "1.0");
            bodyObject.insert("minimumVersion", "0.9");
            bodyObject.insert("channel", "/meta/handshake");
            QList<QVariant> supportedConnectionTypes;
            supportedConnectionTypes.append("long-polling");
            bodyObject.insert("supportedConnectionTypes", supportedConnectionTypes);
            bodyObject.insert("id", ++id);
            body.append(bodyObject);
        }
        else if (!subscribed) {
            QMap<QString, QVariant> bodyObject;
            bodyObject.insert("channel", "/meta/subscribe");
            bodyObject.insert("subscription", QString("/feeds/%1/primary").arg(channelId));
            bodyObject.insert("id", ++id);
            bodyObject.insert("clientId", clientId);
            body.append(bodyObject);
            QMap<QString, QVariant> bodyObject2;
            bodyObject2.insert("channel", "/meta/subscribe");
            bodyObject2.insert("subscription", QString("/feeds/%1/secondary").arg(channelId));
            bodyObject2.insert("id", ++id);
            bodyObject2.insert("clientId", clientId);
            body.append(bodyObject2);
        }
        else {
            url.append("connect");

            QMap<QString, QVariant> bodyObject;
            bodyObject.insert("channel", "/meta/connect");
            bodyObject.insert("connectionType", "long-polling");
            bodyObject.insert("id", ++id);
            bodyObject.insert("clientId", clientId);
            body.append(bodyObject);
        }

        _realtimeSettings.insert("id", id);

        QScriptEngine engine;
        QStringList jsonBody;
        QListIterator<QVariant> it(body);
        while (it.hasNext()) {
            QScriptValueList args;
            args.append(engine.toScriptValue(it.next()));
            jsonBody.append(engine.evaluate("JSON.stringify").call(engine.globalObject(), args).toString());
        }

        OAuthNS::Request *request = new OAuthNS::Request(OAuthNS::Request::POST, url);
        request->setBody(QString("[%1]").arg(jsonBody.join(",")).toUtf8());
        request->exec();

        connect(
            request,
            SIGNAL(responseRecieved(OAuthNS::Response*)),
            this,
            SLOT(_realtimeResponse(OAuthNS::Response*))
        );

        return request;
    }

    void Api::_realtimeResponse(OAuthNS::Response *response)
    {
        response->deleteLater();
        QVariant content = response->getContent();
        QList<QVariant> contentObjects = content.toList();
        QListIterator<QVariant> it(contentObjects);
        while (it.hasNext()) {
            QMap<QString, QVariant> object = it.next().toMap();
            if (object.contains("successful") && object.value("successful").toBool() == true) {
                QString channel = object.contains("channel") ?
                            object.value("channel").toString() : QString();
                if (channel == "/meta/handshake") {
                    _realtimeSettings.insert("clientId", object.value("clientId").toString());
                }
                else if (channel == "/meta/subscribe") {
                    if (!_realtimeSettings.contains("subscriptions") ||
                                    !_realtimeSettings.value("subscriptions").canConvert(QVariant::List)) {
                        _realtimeSettings.insert("subscriptions", QVariantList());
                    }
                    QVariantList subsrciptions = _realtimeSettings.value("subscriptions").toList();
                    subsrciptions.append(object.value("subscription").toString());
                    _realtimeSettings.insert("subscriptions", subsrciptions);
                }
                else if (channel == "/meta/connect") {
                    // let's just reconnect
                }
            }
            else if (object.contains("data") && object.value("data").canConvert(QVariant::Map)) {
                QMap<QString, QVariant> data = object.value("data").toMap();
                if (data.contains("data") && data.value("data").canConvert(QVariant::Map)) {
                    QMap<QString, QVariant> dataData = data.value("data").toMap();
                    if (dataData.contains("messages") && dataData.value("messages").canConvert(QVariant::List)) {
                        emit realtimeMessageList(dataData.value("messages").toList());
                    }
                }
            }
            else {
                _realtimeSettings.clear();
                // emit realtime dead
                return;
            }

            if (!it.hasNext()) {
                QTimer::singleShot(0, this, SLOT(_realtimeRequest()));
            }
        }
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
        QMap<QString, QString> *params)
    {
        //create request
        OAuthNS::Request *request = OAuthNS::Request::fromConsumerAndToken(
            _consumer,
            _accessToken,
            QString("https://www.yammer.com/api/v1/%1.json").arg(resource)
        );

        if (params != 0) {
            QMapIterator<QString, QString> paramIt(*params);
            while (paramIt.hasNext()) {
                paramIt.next();
                request->getUrl()->addQueryItem(paramIt.key(), paramIt.value());
            }
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

        response->deleteLater();
    }
}

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

#ifndef API_H
#define API_H

#include <QObject>
#include "../OAuth/Consumer.h"
#include "../OAuth/Token.h"
#include "../OAuth/Response.h"
#include "../OAuth/Request.h"
#include "Message.h"

namespace YammerNS {

    class Api : public QObject
    {
        Q_OBJECT
    private:
        OAuthNS::Consumer _consumer;
        OAuthNS::Token _accessToken;
        QMap<QString, QVariant> _realtimeSettings;

    private slots:
        OAuthNS::Request* _realtimeRequest();
        void _realtimeResponse(OAuthNS::Response *response);

    public:

        explicit Api(OAuthNS::Consumer consumer, QObject *parent = 0);

        void setAccessToken(OAuthNS::Token token);

        void getRequestToken();

        void getAccessToken(OAuthNS::Token requestToken, QString verifyer);

        bool realtimeSubscribe(QMap<QString, QVariant> settings);

        OAuthNS::Request* get(
            QUrl url,
            QObject* recieverObject,
            const char* recieverMethod
        );

        OAuthNS::Request* get(
            QString resource,
            QObject* recieverObject,
            const char* recieverMethod,
            QMap<QString, QString> *params = 0
        );

        OAuthNS::Request* post(
            QString resource,
            QObject* recieverObject,
            const char* recieverMethod,
            QMap<QString, QString> *params = 0
        );

    signals:
        void requestTokenRecieved(OAuthNS::Token token);
        void accessTokenRecieved(OAuthNS::Token token);
        void realtimeMessageList(QList<QVariant> messageList);

    public slots:
        void responseRecieved(OAuthNS::Response *response);
    };

}

#endif // API_H

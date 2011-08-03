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
* @category OAuth
* @package OAuth
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/lgpl.html GNU GPL
* @link http://yawner.henkehedelund.se/
*/

#ifndef RESPONSE_H
#define RESPONSE_H

#include <QObject>
#include <QNetworkReply>

namespace OAuthNS {

    class Response : public QObject
    {
        Q_OBJECT
    public:

        enum ContentType { PARAMS, XML, JSON, TEXT };

    protected:

        QNetworkReply   *_reply;
        QString         _rawContent;
        QVariant        _content;

    public:

        ContentType     _contentType;

        /**
         *
         * @param QNetworkReply *reply
         * @param QObject *parent
         */
        explicit        Response(QNetworkReply *reply, QObject *parent = 0);

        /**
         *
         * @return QNetworkReply*
         */
        QNetworkReply*  getReplyObject();

        /**
         *
         * @return enum ContentType
         */
        int             getContentType();

        /**
         *
         * @return QVariant
         */
        QString         getRawContent();

        /**
         *
         * @return QVariant
         */
        QVariant        getContent();

    signals:

    public slots:

    };

}

#endif // RESPONSE_H

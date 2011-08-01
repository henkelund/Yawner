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

#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QUrl>
#include <QMap>
#include "Consumer.h"
#include "Token.h"
#include "SignatureMethod.h"

namespace OAuthNS {

    class Request : public QObject
    {
        Q_OBJECT
    protected:
        QString                 _method;
        QUrl                    _url;
        QMap<QString, QString>  _parameters;
        QString                 _baseString;

    public:
        static const QString    VERSION;

        /**
         *
         * @param Qstring method
         * @param QString url
         * @param QMap<QString, QString> parameters
         * @param QObject *parent
         */
        explicit Request(QString method, QString url, QMap<QString, QString> parameters, QObject *parent = 0);

        /**
         *
         * @param Consumer consumer
         * @param Token *token,
         * @param QString method,
         * @param QString url,
         * @param QMap<QString, QString> parameters,
         * @param QObject *parent = 0
         * @return Request*
         */
        static Request* fromConsumerAndToken(
                            Consumer consumer,
                            Token token,
                            QString method,
                            QString url,
                            QMap<QString, QString> *parameters = 0,
                            QObject *parent = 0);

        /**
         * Generates a unique identifier
         *
         * @return QString
         */
        static QString  generateNonce();

        /**
         *
         * @return QString
         */
        static QString  generateTimestamp();

        /**
         * Convenience method for SignatureMethod::buildSignature()
         *
         * @return QString
         */
        QString         buildSignature(
                            SignatureMethod *signatureMethod,
                            Consumer consumer,
                            Token token);

        /**
         *
         * @param QString key
         * @param QString value
         */
        void            setParameter(QString key, QString value);

        /**
         *
         * @param SignatureMethod *signatureMethod
         * @param Consumer consumer
         * @param Token token
         */
        void            signRequest(
                            SignatureMethod *signatureMethod,
                            Consumer consumer,
                            Token token);

        /**
         *
         * @return QString
         */
        QString         toHeader();

        /**
         *
         * @return QString
         */
        QString         toPostData();

        /**
         *
         * @return QUrl
         */
        QUrl            toUrl();

        /**
         *
         * @return QString
         */
        QString         getSignableParameters();

        /**
         *
         * @return QString
         */
        QString         getSignatureBaseString();

        /**
         *
         * @return QString
         */
        QString         getNormalizedHttpUrl();

        /**
         *
         * @param QString baseString
         */
        void            setBaseString(QString baseString);

        /**
         *
         */
        void            exec();

    signals:
        void            responseRecieved();

    public slots:

    };

}

#endif // REQUEST_H

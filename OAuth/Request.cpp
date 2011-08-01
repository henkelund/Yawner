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

#include "Request.h"
#include <QUrl>
#include <QUuid>
#include <QRegExp>
#include <QDateTime>
#include <QStringList>
#include "Util.h"
#include "iostream"

namespace OAuthNS {

    const QString Request::VERSION = QString("1.0");

    Request::Request(QString method, QString url, QMap<QString, QString> parameters, QObject *parent) :
        QObject(parent), _method(method.toUpper()), _url(QUrl(url)), _parameters(parameters), _baseString()
    {
        if (_url.hasQuery()) {
            for (int i = 0; i < _url.queryItems().length(); ++i) {
                QPair<QString, QString> pair = _url.queryItems().at(i);
                if (!_parameters.contains(pair.first)) { // don't overwrite given params
                    _parameters.insert(pair.first, pair.second);
                }
            }
        }
    }

    /* static */
    Request* Request::fromConsumerAndToken(
                                            Consumer consumer,
                                            Token token,
                                            QString method,
                                            QString url,
                                            QMap<QString, QString> *parameters,
                                            QObject *parent)
    {
        QMap<QString, QString> defaultParameters;
        defaultParameters.insert(QString("oauth_version"), VERSION);
        defaultParameters.insert(QString("oauth_nonce"), generateNonce());
        defaultParameters.insert(QString("oauth_timestamp"), generateTimestamp());
        defaultParameters.insert(QString("oauth_consumer_key"), consumer.getKey());

        if (token.getKey().length() > 0) {
            defaultParameters.insert(QString("oauth_token"), token.getKey());
        }

        if (parameters != 0) {
            QMapIterator<QString, QString> it(*parameters);
            while (it.hasNext()) {
                it.next();
                defaultParameters.insert(it.key(), it.value());
            }
        }

        return new Request(method, url, defaultParameters, parent);
    }

    /* static */
    QString Request::generateNonce()
    {
        return QUuid::createUuid().toString()
                // clean from evil chars
                .replace(QRegExp(QString("[^a-z0-9]")), QString(""))
                // substr 16
                .left(16);
    }

    /* static */
    QString Request::generateTimestamp()
    {
        return QString::number(QDateTime::currentDateTime().toTime_t());
    }

    QString Request::buildSignature(SignatureMethod *signatureMethod, Consumer consumer, Token token)
    {
        return signatureMethod->buildSignature(this, consumer, token);
    }

    void Request::setParameter(QString key, QString value)
    {
        _parameters.insert(key, value);
    }

    void Request::signRequest(SignatureMethod *signatureMethod, Consumer consumer, Token token)
    {
        setParameter(QString("oauth_signature_method"), signatureMethod->getName());
        QString signature = buildSignature(signatureMethod, consumer, token);
        setParameter("oauth_signature", signature);
    }

    QString Request::toHeader()
    {
        QString header("OAuth ");
        QStringList paramPairs;
        QMapIterator<QString, QString> it(_parameters);
        while (it.hasNext()) {
            it.next();
            paramPairs.append(
                QString(
                    QUrl::toPercentEncoding(it.key())
                        .append("=\"")
                        .append(QUrl::toPercentEncoding(it.value()))
                        .append("\"")
                )
            );
        }
        return header.append(paramPairs.join(","));
    }

    QString Request::toPostData()
    {
        return Util::buildParameterString(_parameters);
    }

    QUrl Request::toUrl()
    {
        QUrl url(getNormalizedHttpUrl());
        QMapIterator<QString, QString> it(_parameters);
        while (it.hasNext()) {
            it.next();
            url.addQueryItem(it.key().toUtf8(), it.value().toUtf8());
        }
        return url;
    }

    QString Request::getSignableParameters()
    {
        QMap<QString, QString> params(_parameters);
        params.remove(QString("oauth_signature"));
        return Util::buildParameterString(params);
    }

    QString Request::getSignatureBaseString()
    {
        QStringList parts;
        parts.append(QString(QUrl::toPercentEncoding(_method)));
        parts.append(QString(QUrl::toPercentEncoding(getNormalizedHttpUrl())));
        parts.append(QString(QUrl::toPercentEncoding(getSignableParameters())));
        return parts.join(QString("&"));
    }

    QString Request::getNormalizedHttpUrl()
    {
        QString scheme = _url.scheme();
        QString host = _url.host();
        QString path = _url.path();
        int port = _url.port();
        if (port == -1) {
            port = scheme == QString("https") ? 443 : 80;
        }
        if ((scheme == QString("https") && port != 443)
                || (scheme == QString("http") && port != 80)) {
            host = host.append(":").append(QString::number(port));
        }
        return scheme.append("://").append(host).append(path);
    }

    void Request::setBaseString(QString baseString)
    {
        _baseString = baseString;
    }

    void Request::exec()
    {
        emit responseRecieved();
    }
}

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
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link https://github.com/henkelund/Yawner
*/

#include "Request.h"
#include <QUrl>
#include <QUuid>
#include <QRegExp>
#include <QDateTime>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "Util.h"

namespace OAuthNS {

    const QString Request::VERSION = QString("1.0");
    QNetworkAccessManager *Request::_networkAccessManager = 0;

    Request::Request(Method method, QString url, QMap<QString, QString> parameters, QObject *parent) :
        QObject(parent), _method(method), _url(QUrl(url)), _parameters(parameters), _baseString(), _reply(0)
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
                                            QString url,
                                            Method method,
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
    Request* Request::fromConsumer(
                                    Consumer consumer,
                                    QString url,
                                    Method method,
                                    QMap<QString, QString> *parameters,
                                    QObject *parent)
    {
        return fromConsumerAndToken(
                    consumer,
                    Token(QString(""), QString("")),
                    url,
                    method,
                    parameters,
                    parent
        );
    }

    /* static */
    QNetworkAccessManager* Request::getNetworkAccessManager()
    {
        if (_networkAccessManager == 0) {
            _networkAccessManager = new QNetworkAccessManager();
        }
        return _networkAccessManager;
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

    void Request::signRequest(SignatureMethod *signatureMethod, Consumer consumer)
    {
        setParameter(QString("oauth_signature_method"), signatureMethod->getName());
        QString signature = buildSignature(signatureMethod, consumer, Token(QString(""), QString("")));
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
                    QString(it.key())
                        .append("=\"")
                        .append(QString(it.value()))
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

        QString method;
        switch (_method) {
            case POST: method = QString("POST");
                break;
            case PUT: method = QString("PUT");
                break;
            case DELETE: method = QString("DELETE");
                break;
            default: method = QString("GET");
        }

        parts.append(QString(QUrl::toPercentEncoding(method)));
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

    QNetworkReply* Request::exec()
    {
        QNetworkRequest request(_url);
        request.setRawHeader("Authorization", toHeader().toUtf8());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "tex/xml");

        if (_reply != 0 && _reply->parent() == this) {
            delete _reply;
            _reply = 0;
        }

        switch (_method) {
            case POST: _reply = getNetworkAccessManager()->post(request, toPostData().toUtf8());
                break;
            case PUT: _reply = getNetworkAccessManager()->put(request, toPostData().toUtf8());
                break;
            case DELETE: _reply = getNetworkAccessManager()->deleteResource(request);
                break;
            default: _reply = getNetworkAccessManager()->get(request);
        }

        connect(_reply, SIGNAL(finished()), this, SLOT(replyFinished()));
        _reply->setParent(this);

        return _reply;
    }

    void Request::replyFinished()
    {
        if (_reply == 0) {
            return;
        }
        // response steals ownership of _reply
        Response *response = new Response(_reply);
        _reply = 0;

        // set response as parent so this request gets deleted
        // when the response is deleted at the end of the event loop
        this->setParent(response);

        // notify listeners
        emit responseRecieved(response);
    }

    void Request::downloadProgress(qint64 total, qint64 downloaded)
    {
        emit downloadProgress(((float)downloaded)/total);
    }
}

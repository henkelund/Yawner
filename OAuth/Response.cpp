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
* @link https://github.com/henkelund/Yawner
*/

#include "Response.h"
#include "Util.h"

namespace OAuthNS {

    Response::Response(QNetworkReply *reply, QObject *parent) :
        QObject(parent), _reply(reply), _contentType(TEXT)
    {
        // take ownership of the reply
        _reply->setParent(this);

        _rawContent = QString(reply->readAll());

        QVariant contentType = reply->header(QNetworkRequest::ContentTypeHeader);
        if (contentType.isValid()) {
            if (contentType.toString().startsWith(QString("application/x-www-form-urlencoded"))) {
                _contentType = PARAMS;
                QMapIterator<QString, QString> it(Util::parseParameters(_rawContent));
                while (it.hasNext()) {
                    it.next();
                    this->setProperty(it.key().toStdString().c_str(), QVariant(it.value()));
                }
            }
        }
    }

    QNetworkReply* Response::getReplyObject()
    {
        return _reply;
    }

    int Response::getContentType()
    {
        return _contentType;
    }

    QString Response::getRawContent()
    {
        return _rawContent;
    }

    QVariant Response::getContent()
    {
        return _content;
    }

}

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
* @link http://yawner.henkehedelund.se/
*/

#include "User.h"
#include <QVariant>
#include <QFile>
#include <QImage>
#include <QImageWriter>
#include "Yawner.h"

namespace YammerNS {

    int User::_instanceCount = 0;
    QPixmap* User::_defaultSmallImage = 0;

    User::User(QObject *parent) :
        Abstract(parent), _smallImage(0), _hasPendingSmallImageRequest(false)
    {
        ++_instanceCount;
    }

    User::User(int id, QObject *parent) :
        Abstract(id, parent), _smallImage(0), _hasPendingSmallImageRequest(false)
    {
        ++_instanceCount;
    }

    User::~User()
    {
        if (_smallImage != 0) {
            delete _smallImage;
        }
        --_instanceCount;
        if (_instanceCount == 0 && _defaultSmallImage != 0) {
            delete _defaultSmallImage;
            _defaultSmallImage = 0;
        }
    }

    QString User::getName()
    {
        return _data.value(QString("full_name"), QString("")).toString();
    }

    QString User::getUserName()
    {
        return _data.value(QString("name"), QString("")).toString();
    }

    QPixmap User::getSmallImage()
    {
        if (_smallImage == 0) {
            // load from file

            QFile imageFile(_getImageFileName());
            if (imageFile.exists()) {
                _smallImage = new QPixmap(imageFile.fileName());
            }
            else {
                // if not found download from mugshot_url
                QString mugshotUrl = getData(QString("mugshot_url")).toString();
                if (!_hasPendingSmallImageRequest && !mugshotUrl.isEmpty() && !mugshotUrl.contains(QString("no_photo"))) {

                    _hasPendingSmallImageRequest = true;

                    Yawner::getInstance()
                        ->getYammerApi()
                        ->get(
                            QUrl(mugshotUrl),
                            this, SLOT(imageRecieved(OAuthNS::Response*))
                        );
                }

                // while waiting for download -> return default image
                if (_defaultSmallImage == 0) {
                    _defaultSmallImage = new QPixmap(QString(":/icon48.png"));
                }
                return *_defaultSmallImage;
            }
        }
        return *_smallImage;
    }

    QString User::_getImageFileName()
    {
        return Yawner::getInstance()
                ->getYawnerDir().
                absoluteFilePath(
                    QString("%1.user.img").arg(QString::number(getId()))
                );
    }

    void User::imageRecieved(OAuthNS::Response *response)
    {
        response->deleteLater();

        if (response->getRawContent().length() > 0) {
            QImage image;
            image.loadFromData(response->getRawContent());
            QImageWriter writer(
                _getImageFileName(),
                response->getReplyObject()->url().toString().split(QString(".")).last().toUtf8()
            );
            writer.write(image);
            emit dataLoaded(this);
        }

        _hasPendingSmallImageRequest = false;
    }
}

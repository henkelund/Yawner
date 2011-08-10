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
#include "Yawner.h"

namespace YammerNS {

    QPixmap* User::_defaultSmallImage = 0;

    User::User(QObject *parent) :
        Abstract(parent), _smallImage(0)
    {
    }

    User::~User()
    {
        if (_smallImage != 0) {
            delete _smallImage;
        }
    }

    void User::cleanUp()
    {
        if (_defaultSmallImage != 0) {
            delete _defaultSmallImage;
            _defaultSmallImage = 0;
        }
    }

    int User::getId()
    {
        return _data.value(QString("id"), QVariant(0)).toInt();
    }

    QPixmap User::getSmallImage()
    {
        if (_smallImage == 0) {
            // load from file
            QFile imageFile(
                Yawner::getInstance()
                ->getYawnerDir().
                absoluteFilePath(
                    QString("%1.user.jpg")
                        .arg(QString::number(getId()))
                )
            );

            if (imageFile.exists()) {
                _smallImage = new QPixmap(imageFile.fileName());
            }
            else {
                // if not found download from mugshot_url
                // while waiting for download -> return default image
                if (_defaultSmallImage == 0) {
                    _defaultSmallImage = new QPixmap(QString(":/icon.svg"));
                }
                return *_defaultSmallImage;
            }
        }
        return *_smallImage;
    }

}

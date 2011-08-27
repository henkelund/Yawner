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

#ifndef USER_H
#define USER_H

#include "Abstract.h"
#include <QObject>
#include <QScriptValue>
#include <QPixmap>
#include "../OAuth/Response.h"

namespace YammerNS {

    class User : public Abstract
    {
        Q_OBJECT

    private:
        static int      _instanceCount;
        QPixmap         *_smallImage;
        bool            _hasPendingSmallImageRequest;
        static QPixmap  *_defaultSmallImage;

        QString _getImageFileName();

    public:
        explicit User(QObject *parent = 0);

        explicit User(int id, QObject *parent = 0);

        virtual ~User();

        QString getName();

        QString getUserName();

        QPixmap getSmallImage();

    signals:

    public slots:
        void imageRecieved(OAuthNS::Response *response);
    };

}

#endif // USER_H

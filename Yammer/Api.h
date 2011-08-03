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
* @license http://www.gnu.org/licenses/lgpl.html GNU GPL
* @link https://github.com/henkelund/Yawner
*/

#ifndef API_H
#define API_H

#include <QObject>
#include "../OAuth/Consumer.h"
#include "../OAuth/Response.h"

namespace Yammer {

    class Api : public QObject
    {
        Q_OBJECT
    protected:
        OAuthNS::Consumer _consumer;

    public:
        explicit Api(OAuthNS::Consumer consumer, QObject *parent = 0);

        void getRequestToken();

        void call(const char *method);

    signals:
        void requestTokenRecieved(QString token, QString secret);

    public slots:
        void responseRecieved(OAuthNS::Response *response);

    };

}

#endif // API_H

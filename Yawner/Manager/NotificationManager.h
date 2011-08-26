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
* @category Yawner/Manager
* @package Yawner/Manager
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link https://github.com/henkelund/Yawner
*/

#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "../Manager.h"
#include <QSystemTrayIcon>
#include "Yammer/Message.h"

namespace YawnerNS {
    namespace ManagerNS {

        class NotificationManager : public YawnerNS::Manager
        {
            Q_OBJECT
        protected:

            QSystemTrayIcon *_icon;

            virtual void _init();

        public:
            explicit NotificationManager(QObject *parent = 0);

            void show(QString title, QString message);

            void show(YammerNS::Message *message);

        signals:

        public slots:

        };

    }
}

#endif // NOTIFICATIONMANAGER_H

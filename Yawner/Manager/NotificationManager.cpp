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

#include <QtGlobal>
#ifdef Q_WS_X11
#include <libnotify/notify.h>
#endif
#include "NotificationManager.h"

namespace YawnerNS {
    namespace ManagerNS {

        NotificationManager::NotificationManager(QObject *parent) :
            YawnerNS::Manager(parent), _icon(0)
        {
        }

        void NotificationManager::_init()
        {
            _icon = new QSystemTrayIcon(QIcon(QPixmap(QString(":/icon16.png"))), this);
            _icon->show();
#ifdef Q_WS_X11
            notify_init("Yawner");
#else
#endif
        }

        void NotificationManager::show(QString title, QString message)
        {
#ifdef Q_WS_X11
            NotifyNotification *notification =
                    notify_notification_new(title.toUtf8(), message.toUtf8(), NULL, NULL);
            notify_notification_set_timeout(notification, 3000);
            notify_notification_show(notification, NULL);
            g_object_unref(notification);
#else
            _icon->showMessage(title, message, QSystemTrayIcon::Information, 3000);
#endif
        }
    }
}

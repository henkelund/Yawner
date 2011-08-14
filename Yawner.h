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
* @category Yawner
* @package Yawner
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link https://github.com/henkelund/Yawner
*/

#ifndef YAWNER_H
#define YAWNER_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QSettings>
#include "Yammer/Api.h"
#include "OAuth/Consumer.h"
#include "OAuth/Token.h"

class Yawner;

#include "Yawner/Manager/UserManager.h"
#include "Yawner/Manager/MessageManager.h"
#include "Yawner/Manager/NotificationManager.h"

class Yawner : public QObject
{
    Q_OBJECT

private:

    static Yawner *_instance;
    QSettings *_settings;
    YammerNS::Api *_yammerApi;
    YawnerNS::ManagerNS::UserManager *_userManager;
    YawnerNS::ManagerNS::MessageManager *_messageManager;
    YawnerNS::ManagerNS::NotificationManager *_notificationManager;

    explicit Yawner();

public:

    ~Yawner();

    static Yawner* getInstance();

    YammerNS::Api* getYammerApi();

    QSettings* getSettings();

    QDir getYawnerDir();

    OAuthNS::Consumer getConsumer();

    void setConsumer(OAuthNS::Consumer consumer);

    OAuthNS::Token getAccessToken();

    void setAccessToken(OAuthNS::Token accessToken);

    YawnerNS::ManagerNS::UserManager* getUserManager();

    YawnerNS::ManagerNS::MessageManager* getMessageManager();

    YawnerNS::ManagerNS::NotificationManager* getNotificationManager();

    QString getFileContents(QString filename);

};

#endif // YAWNER_H

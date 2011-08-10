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

#include "Yawner.h"
#include "Yammer/Api.h"
#include "stdlib.h"

Yawner* Yawner::_instance = 0;

Yawner::Yawner() :
    QObject(0),
    _settings(new QSettings(QString("Yawner"), QString("Yawner"), this)),
    _yammerApi(0),
    _userManager(0)
{
    _yammerApi = new YammerNS::Api(getConsumer(), this);
}

Yawner::~Yawner()
{
}

Yawner* Yawner::getInstance()
{
    if (_instance == 0) {
        _instance = new Yawner();
    }
    return _instance;
}

YammerNS::Api* Yawner::getYammerApi()
{
    return _yammerApi;
}

QSettings* Yawner::getSettings()
{
    return _settings;
}

QDir Yawner::getYawnerDir()
{
    QDir home = QDir::home();
    QString yawnerDirName(".Yawner");
    if (!home.cd(yawnerDirName)) {
        home.mkdir(yawnerDirName);

        if (!home.cd(yawnerDirName)) {
            qDebug("Can't cd to Yawner dir");
            exit(-1);
        }
    }
    return home;
}

OAuthNS::Consumer Yawner::getConsumer()
{
    return OAuthNS::Consumer(
        getSettings()->value(QString("consumer/key")).toString(),
        getSettings()->value(QString("consumer/secret")).toString()
    );
}

void Yawner::setConsumer(OAuthNS::Consumer consumer)
{
    getSettings()->setValue(QString("consumer/key"), QVariant(consumer.getKey()));
    getSettings()->setValue(QString("consumer/secret"), QVariant(consumer.getSecret()));
}

OAuthNS::Token Yawner::getAccessToken()
{
    return OAuthNS::Token(
        getSettings()->value(QString("access_token/key")).toString(),
        getSettings()->value(QString("access_token/secret")).toString()
    );
}

void Yawner::setAccessToken(OAuthNS::Token accessToken)
{
    getSettings()->setValue(QString("access_token/key"), QVariant(accessToken.getKey()));
    getSettings()->setValue(QString("access_token/secret"), QVariant(accessToken.getSecret()));
}

YawnerNS::ManagerNS::UserManager* Yawner::getUserManager()
{
    if (_userManager == 0) {
        _userManager = new YawnerNS::ManagerNS::UserManager(this);
        _userManager->init();
    }
    return _userManager;
}

YawnerNS::ManagerNS::MessageManager* Yawner::getMessageManager()
{
    if (_messageManager == 0) {
        _messageManager = new YawnerNS::ManagerNS::MessageManager(this);
        _messageManager->init();
    }
    return _messageManager;
}

QString Yawner::getFileContents(QString filename)
{
    QFile file(getYawnerDir().absoluteFilePath(filename));
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QString contents(file.readAll());
        file.close();
        return contents;
    }
    return QString(); // return null string
}

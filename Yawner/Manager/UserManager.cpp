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
* @link http://yawner.henkehedelund.se/
*/

#include "UserManager.h"
#include <QDir>
#include <QScriptEngine>
#include "Yawner.h"

namespace YawnerNS {
    namespace ManagerNS {

        UserManager::UserManager(QObject *parent) :
            YawnerNS::Manager(parent), _isLoaded(false), _userIndex()
        {
        }

        void UserManager::init()
        {

        }

        void UserManager::_assertLoaded()
        {
            if (_isLoaded == true) {
                return;
            }
            load();
            _isLoaded = true;
        }

        void UserManager::load()
        {
            QDir dir = _yawner()->getYawnerDir();
            QStringList filters;
            filters.append(QString("*.user.json"));
            QStringList files = dir.entryList(filters, QDir::Files);
            QStringListIterator it(files);
            while (it.hasNext()) {
                QString file = it.next();
                int uid = file.split(QString(".")).at(0).toInt();

                QString json = _yawner()->getFileContents(file);
                YammerNS::User* usr;
                if (_userIndex.contains(uid)) {
                    usr = _userIndex.value(uid);
                }
                else {
                    usr = new YammerNS::User(this);
                    _userIndex.insert(uid, usr);
                }

                if (!YammerNS::User::loadJson(usr, json)) {
                    _userIndex.remove(uid);
                    delete usr;
                    qDebug(QString("Couldn't load data: %1").arg(json).toStdString().c_str());
                }
            }
        }

        YammerNS::User* UserManager::getUserById(int id)
        {
            _assertLoaded();
            if (_userIndex.contains(id)) {
                return _userIndex.value(id);
            }
            return 0;
        }

        void UserManager::userRecieved(OAuthNS::Response* response)
        {
            qDebug(response->getRawContent().toStdString().c_str());
            qDebug(QString::number(response->getReplyObject()->error()).toStdString().c_str());
        }

    }
}

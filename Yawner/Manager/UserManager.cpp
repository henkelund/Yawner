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
#include <QTimer>
#include "Yawner.h"

namespace YawnerNS {
    namespace ManagerNS {

        UserManager::UserManager(QObject *parent) :
            YawnerNS::Manager(parent), _userIndex()
        {
        }

        UserManager::~UserManager()
        {
            QList<YammerNS::Abstract*> usersToSave;
            QMapIterator<int, YammerNS::User*> it(_userIndex);
            while (it.hasNext()) {
                it.next();
                usersToSave.append((YammerNS::User*) it.value());
            }
            _storeObjectList("users", usersToSave);
        }

        void UserManager::init()
        {
            QVariantList users = _loadStoredObjectList("users");

            if (!users.isEmpty()) {
                updateUsersData(users);
                if (_userIndex.isEmpty()) {
                    fetchUsers();
                }
            }
            else {
                fetchUsers();
            }
        }

        void UserManager::fetchUsers()
        {
            _yawner()
                ->getYammerApi()
                ->get(
                    "users",
                    this, SLOT(usersRecieved(OAuthNS::Response*))
                );
        }

        QMap<int, YammerNS::User*> UserManager::getUserIndex()
        {
            return _userIndex;
        }

        YammerNS::User* UserManager::getUserById(int id, bool *created)
        {
            if (!_userIndex.contains(id)) {
                _userIndex.insert(id, new YammerNS::User(id, this));
                if(created != 0) {
                    (*created) = true;
                }
            }
            else if(created != 0) {
                (*created) = false;
            }
            return _userIndex.value(id);
        }

        QList<YammerNS::User*> UserManager::getUsersByUserName(QRegExp pattern)
        {
            QList<YammerNS::User*> matches;
            QMapIterator<int, YammerNS::User*> it(_userIndex);
            while (it.hasNext()) {
                YammerNS::User* user = it.next().value();
                if (pattern.indexIn(user->getData("name").toString()) >= 0) {
                    matches.append(user);
                }
            }
            return matches;
        }

        void UserManager::updateUsersData(QList<QVariant> userList)
        {
            QList<int> newUserIds;

            QListIterator<QVariant> it(userList);
            while (it.hasNext()) {

                QVariant userListItem = it.next();
                if (userListItem.canConvert(QVariant::Map)) {

                    QMap<QString, QVariant> userData = userListItem.toMap();
                    if (userData.contains("id") &&
                            userData.value("id").canConvert(QVariant::Int)) {

                        int id = userData.value("id").toInt();
                        if (id > 0) {
                            bool created = false;
                            YammerNS::User *user = getUserById(id, &created);
                            user->load(userData);
                            if (created == true) {
                                newUserIds.append(id);
                            }
                        }
                    }
                }
            }

            if (newUserIds.count() > 0) {
                emit newUsersLoaded(newUserIds);
            }
        }

        void UserManager::usersRecieved(OAuthNS::Response* response)
        {
            response->deleteLater();

            if (response->getContent().canConvert(QVariant::List)) {
                updateUsersData(response->getContent().toList());
            }
        }
    }
}

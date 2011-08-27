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

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "../Manager.h"
#include "../../Yammer/User.h"
#include "../../OAuth/Response.h"

namespace YawnerNS {
    namespace ManagerNS {

        class UserManager : public YawnerNS::Manager
        {
            Q_OBJECT

        protected:

            QMap<int, YammerNS::User*> _userIndex;

        public:

            explicit UserManager(QObject *parent);

            virtual ~UserManager();

            void init();

            QMap<int, YammerNS::User*> getUserIndex();

            YammerNS::User* getUserById(int id, bool *created = 0);

            QList<YammerNS::User*> getUsersByUserName(QRegExp pattern);

        signals:
            void newUsersLoaded(QList<int> newUserIds);

        public slots:
            void fetchUsers();
            void updateUsersData(QList<QVariant> userList);
            void usersRecieved(OAuthNS::Response* response);

        };

    }
}

#endif // USERMANAGER_H

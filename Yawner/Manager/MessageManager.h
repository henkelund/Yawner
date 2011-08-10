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

#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "../Manager.h"
#include "../../Yammer/Message.h"
#include "../../OAuth/Response.h"

namespace YawnerNS {
    namespace ManagerNS {

        class MessageManager : public YawnerNS::Manager
        {
            Q_OBJECT
        protected:

            /**
             * This map variable should hold all the message objects for the application
             * and this manager should be the parent of those objects
             *
             * @var QMap<int, Message*>
             */
            QMap<int, YammerNS::Message*> _messageIndex;

        public:
            explicit MessageManager(QObject *parent = 0);

            void init();

            YammerNS::Message* getMessageById(int id, bool *created = 0);

        signals:
            void newMessagesLoaded(QList<int> messageIds);

        public slots:
            void messagesRecieved(OAuthNS::Response* response);

        };

    }
}

#endif // MESSAGEMANAGER_H

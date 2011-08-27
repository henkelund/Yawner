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
        private:

            /**
             * This map variable should hold all the message objects for the application
             * and this manager should be the parent of those objects
             *
             * @var QMap<int, Message*>
             */
            QMap<int, YammerNS::Message*>   _messageIndex;

            QList<YammerNS::Message*>       _cronoIndex;

            QList<int>                      _lastLoadIds;

            int                             _replyToMessageId;

            void _buildCronoIndex();

        public:

            enum Filter {None, OlderThan, NewerThan};

            explicit MessageManager(QObject *parent = 0);

            virtual ~MessageManager();

            void init();

            QMap<int, YammerNS::Message*> getMessageIndex();

            QList<int> getLastLoadIds();

            YammerNS::Message* getNewestMessage();

            YammerNS::Message* getOldestMessage();

            YammerNS::Message* getRepliedToMessage(YammerNS::Message *reply);

            YammerNS::Message* getThreadStarterMessage(YammerNS::Message *reply);

            YammerNS::Message* getMessageById(int id, bool *created = 0);

            QList<YammerNS::Message*> getMessagesByAttribute(QString key, QVariant value);

            QList<YammerNS::Message*> getThreadMessages(int threadId);

            void requestMessages(Filter filter = None, QVariant value = QVariant());

            int getReplyToMessageId();

            void setReplyToMessage(YammerNS::Message *message);

            void postMessage(QString message);

        signals:
            void newMessagesLoaded(QList<int> messageIds);
            void replyToMessageChanged(int id);

        public slots:
            void requestThreadMessages(int threadId);
            void updateMessagesData(QList<QVariant> messageList);
            void messagesRecieved(OAuthNS::Response* response);

        };

    }
}

#endif // MESSAGEMANAGER_H

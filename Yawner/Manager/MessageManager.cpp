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

#include "MessageManager.h"
#include "Yawner.h"
#include "../../OAuth/Request.h"

namespace YawnerNS {
    namespace ManagerNS {

        MessageManager::MessageManager(QObject *parent) :
            YawnerNS::Manager(parent), _messageIndex()
        {
        }

        void MessageManager::init()
        {
            fetchMessages();
        }

        void MessageManager::fetchMessages()
        {
            OAuthNS::Request *request = _yawner()
                ->getYammerApi()
                ->get(
                    "https://www.yammer.com/api/v1/messages.json",
                    this, SLOT(messagesRecieved(OAuthNS::Response*))
                );

        }

        YammerNS::Message* MessageManager::getMessageById(int id, bool *created)
        {
            if (!_messageIndex.contains(id)) {
                _messageIndex.insert(id, new YammerNS::Message(this));
                if(created != 0) {
                    (*created) = true;
                }
            }
            else if(created != 0) {
                (*created) = false;
            }
            return _messageIndex.value(id);
        }

        void MessageManager::messagesRecieved(OAuthNS::Response* response)
        {
            response->deleteLater();

            if (response->getContent().canConvert(QVariant::Map)) {

                QList<int> newMessageIds;

                QMap<QString, QVariant> responseObject = response->getContent().toMap();
                if (responseObject.contains("messages") &&
                        responseObject.value("messages").canConvert(QVariant::List)) {

                    QList<QVariant> messageList = responseObject.value("messages").toList();
                    QListIterator<QVariant> it(messageList);
                    while (it.hasNext()) {

                        QVariant messageListItem = it.next();
                        if (messageListItem.canConvert(QVariant::Map)) {

                            QMap<QString, QVariant> messageData = messageListItem.toMap();
                            if (messageData.contains("id") &&
                                    messageData.value("id").canConvert(QVariant::Int)) {

                                int id = messageData.value("id").toInt();
                                if (id > 0) {
                                    bool created = false;
                                    YammerNS::Message *message = getMessageById(id, &created);
                                    message->load(messageData);
                                    if (created == true) {
                                        newMessageIds.append(id);
                                    }
                                }
                            }
                        }
                    }
                }

                if (newMessageIds.count() > 0) {
                    emit newMessagesLoaded(newMessageIds);
                }
            }
        }
    }
}

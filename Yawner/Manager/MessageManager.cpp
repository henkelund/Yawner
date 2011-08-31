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
#include <QScriptEngine>
#include <QScriptValue>
#include "Yawner.h"
#include "../../OAuth/Request.h"

namespace YawnerNS {
    namespace ManagerNS {

        MessageManager::MessageManager(QObject *parent) :
            YawnerNS::Manager(parent),
            _messageIndex(),
            _cronoIndex(),
            _lastLoadIds(),
            _replyToMessageId(-1)
        {
        }

        MessageManager::~MessageManager()
        {
            QList<YammerNS::Abstract*> messagesToSave;
            int maxNumToStore = 100; // hard coded for now
            for (int i = 0; i < maxNumToStore && i < _cronoIndex.length(); ++i) {
                messagesToSave.append((YammerNS::Abstract*) _cronoIndex.at(i));
            }
            _storeObjectList("messages", messagesToSave);
        }

        void MessageManager::init()
        {
            connect(
                _yawner()->getYammerApi(),
                SIGNAL(realtimeMessageList(QList<QVariant>)),
                this,
                SLOT(updateMessagesData(QList<QVariant>))
            );

            QVariantList messages = _loadStoredObjectList("messages");

            if (!messages.isEmpty()) {
                updateMessagesData(messages);
                if (_messageIndex.count() > 0) {
                    //requestMessages(NewerThan, getNewestMessage()->getId());
                }
                else {
                    requestMessages();
                }
            }
            else {
                requestMessages();
            }
        }

        void MessageManager::requestMessages(Filter filter, QVariant value)
        {
            QMap<QString, QString> params;
            switch (filter) {
                case OlderThan: {
                    if (value.canConvert(QVariant::Int)) {
                        params.insert("older_than", value.toString());
                    }
                }
                break;
                case NewerThan: {
                    if (value.canConvert(QVariant::Int)) {
                        params.insert("newer_than", value.toString());
                    }
                }
                break;
                default: {}
            }

            _yawner()
                ->getYammerApi()
                ->get(
                    "messages",
                    this, SLOT(messagesRecieved(OAuthNS::Response*)),
                    &params
                );
        }

        void MessageManager::requestThreadMessages(int threadId)
        {
            _yawner()
                ->getYammerApi()
                ->get(
                    QString("messages/in_thread/%1").arg(QString::number(threadId)),
                    this, SLOT(messagesRecieved(OAuthNS::Response*))
                );
        }

        QMap<int, YammerNS::Message*> MessageManager::getMessageIndex()
        {
            return _messageIndex;
        }

        void MessageManager::_buildCronoIndex()
        {
            QList<YammerNS::Message*> newIndex = _messageIndex.values();
            qSort(newIndex.begin(), newIndex.end(), YammerNS::message_is_newer_than);
            _cronoIndex = newIndex;
        }

        QList<int> MessageManager::getLastLoadIds()
        {
            return _lastLoadIds;
        }

        YammerNS::Message* MessageManager::getNewestMessage()
        {
            if (_cronoIndex.length() > 0) {
                return _cronoIndex.first();
            }
            return 0;
        }

        YammerNS::Message* MessageManager::getOldestMessage()
        {
            if (_cronoIndex.length() > 0) {
                return _cronoIndex.last();
            }
            return 0;
        }

        YammerNS::Message* MessageManager::getRepliedToMessage(YammerNS::Message *reply)
        {
            if (reply != 0 && reply->getRepliedToId() > 0) {
                bool created = false;
                YammerNS::Message *message = getMessageById(reply->getRepliedToId(), &created);
                if (created == true) {
                    requestThreadMessages(reply->getThreadStarterId());
                }
                return message;
            }
            return 0;
        }

        YammerNS::Message* MessageManager::getThreadStarterMessage(YammerNS::Message *reply)
        {
            if (reply != 0 && reply->getThreadStarterId() > 0) {
                bool created = false;
                YammerNS::Message *message = getMessageById(reply->getThreadStarterId(), &created);
                if (created == true) {
                    requestThreadMessages(reply->getThreadStarterId());
                }
                return message;
            }
            return 0;
        }

        YammerNS::Message* MessageManager::getMessageById(int id, bool *created)
        {
            if (!_messageIndex.contains(id)) {
                _messageIndex.insert(id, new YammerNS::Message(id, this));
                if(created != 0) {
                    (*created) = true;
                }
            }
            else if(created != 0) {
                (*created) = false;
            }
            return _messageIndex.value(id);
        }

        QList<YammerNS::Message*> MessageManager::getMessagesByAttribute(QString key, QVariant value)
        {
            QList<YammerNS::Message*> messages;
            QMapIterator<int, YammerNS::Message*> it(_messageIndex);
            while (it.hasNext()) {
                it.next();
                QVariant messageValue = it.value()->getData(key);
                if (messageValue.isValid() && messageValue == value) {
                    messages.append(it.value());
                }
            }
            return messages;
        }

        QList<YammerNS::Message*> MessageManager::getThreadMessages(int threadId)
        {
            return getMessagesByAttribute(QString("thread_id"), QVariant(threadId));
        }

        int MessageManager::getReplyToMessageId()
        {
            return _replyToMessageId;
        }

        void MessageManager::setReplyToMessage(YammerNS::Message *message)
        {
            if (message == 0) {
                _replyToMessageId = -1;
            }
            else {
                _replyToMessageId = message->getId();
            }
            emit replyToMessageChanged(_replyToMessageId);
        }

        void MessageManager::postMessage(QString message)
        {
            QMap<QString, QString> params;
            params.insert("body", message);
            if (getReplyToMessageId() > 0) {
                params.insert("replied_to_id", QString::number(getReplyToMessageId()));
                setReplyToMessage(0);
            }

            _yawner()
                ->getYammerApi()
                ->post(
                    "messages",
                    this, SLOT(messagesRecieved(OAuthNS::Response*)),
                    &params
                );
        }

        void MessageManager::updateMessagesData(QList<QVariant> messageList)
        {
            QList<int> newMessageIds;
            QListIterator<QVariant> it(messageList);
            while (it.hasNext()) {

                QVariant messageListItem = it.next();
                if (messageListItem.canConvert(QVariant::Map)) {

                    QMap<QString, QVariant> messageData = messageListItem.toMap();
                    if (messageData.contains("id") &&
                            messageData.value("id").canConvert(QVariant::Int)) {

                        int id = messageData.value("id").toInt();
                        if (id > 0) {
                            YammerNS::Message *message = getMessageById(id);
                            bool firstLoad = false;
                            message->load(messageData, &firstLoad);
                            if (firstLoad == true) {
                                newMessageIds.append(id);
                            }
                        }
                    }
                }
            }

            _buildCronoIndex();

            if (newMessageIds.count() > 0) {
                _lastLoadIds = newMessageIds;
                emit newMessagesLoaded(newMessageIds);
            }
        }

        void MessageManager::messagesRecieved(OAuthNS::Response* response)
        {
            response->deleteLater();
            if (response->getContent().canConvert(QVariant::Map)) {

                QMap<QString, QVariant> responseObject = response->getContent().toMap();

                if (responseObject.contains("meta") && responseObject.value("meta").canConvert(QVariant::Map)) {
                    QMap<QString, QVariant> meta = responseObject.value("meta").toMap();
                    if (meta.contains("realtime") && meta.value("realtime").canConvert(QVariant::Map)) {
                        QMap<QString, QVariant> realtime = meta.value("realtime").toMap();
                        _yawner()->getYammerApi()->realtimeSubscribe(realtime);
                    }
                }

                if (responseObject.contains("messages") &&
                        responseObject.value("messages").canConvert(QVariant::List)) {

                    QList<QVariant> messageList = responseObject.value("messages").toList();
                    updateMessagesData(messageList);
                }
            }
        }
    }
}

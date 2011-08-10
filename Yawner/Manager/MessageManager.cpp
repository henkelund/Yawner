#include "MessageManager.h"
#include "Yawner.h"

namespace YawnerNS {
    namespace ManagerNS {

        MessageManager::MessageManager(QObject *parent) :
            YawnerNS::Manager(parent), _messageIndex()
        {
        }

        void MessageManager::init()
        {
            _yawner()
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

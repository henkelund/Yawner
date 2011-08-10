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

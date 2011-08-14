#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "../Manager.h"
#include <QSystemTrayIcon>

namespace YawnerNS {
    namespace ManagerNS {

        class NotificationManager : public YawnerNS::Manager
        {
            Q_OBJECT
        protected:

            QSystemTrayIcon *_icon;

            virtual void _init();

        public:
            explicit NotificationManager(QObject *parent = 0);

            void show(QString title, QString message);

        signals:

        public slots:

        };

    }
}

#endif // NOTIFICATIONMANAGER_H

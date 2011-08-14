#include <QtGlobal>
#ifdef Q_WS_X11
#include <libnotify/notify.h>
#endif
#include "NotificationManager.h"

namespace YawnerNS {
    namespace ManagerNS {

        NotificationManager::NotificationManager(QObject *parent) :
            YawnerNS::Manager(parent), _icon(0)
        {
        }

        void NotificationManager::_init()
        {
            _icon = new QSystemTrayIcon(QIcon(QPixmap(QString(":/icon16.png"))), this);
#ifdef Q_WS_X11
            notify_init("Yawner");
#else
#endif
        }

        void NotificationManager::show(QString title, QString message)
        {
#ifdef Q_WS_X11
            NotifyNotification *notification =
                    notify_notification_new(title.toUtf8(), message.toUtf8(), NULL, NULL);
            notify_notification_set_timeout(notification, 3000);
            notify_notification_show(notification, NULL);
            g_object_unref(notification);
#else
            _icon->showMessage(title, message, QSystemTrayIcon::Information, 3000);
#endif
        }
    }
}

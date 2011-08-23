#ifndef BODYWIDGET_H
#define BODYWIDGET_H

#include <QStackedWidget>
#include "Yawner/Ui/View/Snapshot.h"

namespace YawnerNS {
    namespace UiNS {

        class BodyWidget : public QStackedWidget
        {
            Q_OBJECT
        private:
            YawnerNS::UiNS::ViewNS::Snapshot *_currentSnapshot;

        public:
            explicit BodyWidget(QWidget *parent = 0);

            void showView(QWidget *widget, bool animate = true);

        signals:

        public slots:
            void snapshotRemoved(QWidget *target);

        };

    }
}

#endif // BODYWIDGET_H

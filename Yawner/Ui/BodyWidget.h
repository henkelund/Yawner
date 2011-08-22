#ifndef BODYWIDGET_H
#define BODYWIDGET_H

#include <QStackedWidget>
#include "Yawner/Ui/View/AbstractView.h"
#include "Yawner/Ui/View/Snapshot.h"

namespace YawnerNS {
    namespace UiNS {

        class BodyWidget : public QStackedWidget
        {
            Q_OBJECT
        public:
            explicit BodyWidget(QWidget *parent = 0);

            void showView(YawnerNS::UiNS::ViewNS::AbstractView *widget, bool animate = true);

        signals:

        public slots:
            void snapshotRemoved(QWidget *after);

        };

    }
}

#endif // BODYWIDGET_H

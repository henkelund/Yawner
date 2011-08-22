#include "BodyWidget.h"
#include <QStackedLayout>
#include <QPropertyAnimation>
#include <QMutex>
#include "Yawner/Ui/View/Snapshot.h"

namespace YawnerNS {
    namespace UiNS {

        BodyWidget::BodyWidget(QWidget *parent) :
            QStackedWidget(parent)
        {
            QStackedLayout *layout = static_cast<QStackedLayout*>(this->layout());
            layout->setStackingMode(QStackedLayout::StackAll);
        }

        void BodyWidget::showView(YawnerNS::UiNS::ViewNS::AbstractView *widget, bool animate)
        {
            if (widget == currentWidget()) {
                return;
            }

            QMutex mutex;
            mutex.lock();

            YawnerNS::UiNS::ViewNS::Snapshot *snapshot = 0;
            if (animate) {
                // create a snapshot
                snapshot = new YawnerNS::UiNS::ViewNS::Snapshot(this, widget);
                connect(snapshot, SIGNAL(disposed(QWidget*)), this, SLOT(snapshotRemoved(QWidget*)));
                QList<YawnerNS::UiNS::ViewNS::Snapshot*> oldSnapshots =
                        findChildren<YawnerNS::UiNS::ViewNS::Snapshot*>();
                QListIterator<YawnerNS::UiNS::ViewNS::Snapshot*> it(oldSnapshots);
                while (it.hasNext()) {
                    it.next()->dispose();
                }
            }

            // raise the requested widget to the top
            setCurrentWidget(widget);

            if (animate) {
                // add and raise the snapshot, making the requested widget second
                addWidget(snapshot);
                setCurrentWidget(snapshot);
                QPropertyAnimation *animation = new QPropertyAnimation(snapshot, "maximumWidth");
                animation->setDuration(500);
                animation->setEasingCurve(QEasingCurve::OutQuad);
                animation->setStartValue(snapshot->width());
                animation->setEndValue(0);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                connect(animation, SIGNAL(finished()), snapshot, SLOT(dispose()));
            }
            else {
                widget->setFocus();
            }
        }

        void BodyWidget::snapshotRemoved(QWidget *after)
        {
            setCurrentWidget(after);
            after->setFocus();
        }
    }
}

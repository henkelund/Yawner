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
* @category Yawner/Ui
* @package Yawner/Ui
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link http://yawner.henkehedelund.se/
*/

#include "BodyWidget.h"
#include <QStackedLayout>
#include <QPropertyAnimation>
#include "Yawner/Ui/View/Snapshot.h"

namespace YawnerNS {
    namespace UiNS {

        BodyWidget::BodyWidget(QWidget *parent) :
            QStackedWidget(parent), _currentSnapshot(0)
        {
            QStackedLayout *layout = static_cast<QStackedLayout*>(this->layout());
            layout->setStackingMode(QStackedLayout::StackAll);
        }

        void BodyWidget::showView(QWidget *widget, bool animate)
        {
            if (widget == currentWidget()) {
                return;
            }

            if (animate) {
                // create a snapshot
                _currentSnapshot = new YawnerNS::UiNS::ViewNS::Snapshot(widget, this);
                if (layout()->indexOf(widget) < layout()->indexOf(currentWidget())) {
                    _currentSnapshot->setDirection(YawnerNS::UiNS::ViewNS::Snapshot::Right);
                }
                connect(_currentSnapshot, SIGNAL(disposed(QWidget*)), this, SLOT(snapshotRemoved(QWidget*)));
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
                addWidget(_currentSnapshot);
                setCurrentWidget(_currentSnapshot);
                QPropertyAnimation *animation = new QPropertyAnimation(_currentSnapshot, "progress");
                animation->setDuration(500);
                animation->setEasingCurve(QEasingCurve::OutQuad);
                animation->setStartValue(0.f);
                animation->setEndValue(1.f);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                connect(animation, SIGNAL(finished()), _currentSnapshot, SLOT(dispose()));
            }
            else {
                widget->setFocus();
            }
        }

        void BodyWidget::snapshotRemoved(QWidget *target)
        {
            setCurrentWidget(target);
            target->setFocus();
        }
    }
}

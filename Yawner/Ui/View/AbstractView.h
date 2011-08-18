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
* @category Yawner/Ui/View
* @package Yawner/Ui/View
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link http://yawner.henkehedelund.se/
*/

#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QScrollArea>

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {

            class AbstractView : public QScrollArea
            {
                Q_OBJECT
            private:
                bool _isInitialized;

            protected:
                virtual void _init();

            public:
                explicit AbstractView(QWidget *parent = 0);

                void init();

                virtual void beforeShow();
                virtual void beforeHide();

            signals:

            public slots:
                void paintEvent(QPaintEvent *event);
                void minimized();
                void maximized();
            };

        }
    }
}

#endif // ABSTRACTVIEW_H

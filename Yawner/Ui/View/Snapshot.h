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

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QWidget>
#include <QImage>

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {

            class Snapshot : public QWidget
            {
                Q_OBJECT

            private:
                QWidget *_before;
                QWidget *_after;
                QImage  _image;
                bool    _disposed;

            public:
                explicit Snapshot(QWidget *before, QWidget *after, QWidget *parent = 0);

            signals:
                void disposed(QWidget *after);

            public slots:
                void paintEvent(QPaintEvent *parent);
                void dispose();

            };

        }
    }
}

#endif // SNAPSHOT_H

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
* @link https://github.com/henkelund/Yawner
*/

#include "MessageText.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QLinearGradient>

namespace YawnerNS {
    namespace UiNS {

        MessageText::MessageText(QWidget *parent) :
            QWidget(parent)
        {
            setAutoFillBackground(false);
        }

        void MessageText::paintEvent(QPaintEvent *e)
        {
            QPainter painter(this);

            painter.setRenderHint(QPainter::Antialiasing, true);
            // dark path
            QPainterPath darkPath;
            // arrow
            darkPath.moveTo(10, 10);
            darkPath.quadTo(10, 17, 0, 24);
            darkPath.quadTo(10, 31, 10, 38);
            darkPath.lineTo(10, 10);
            // area
            darkPath.addRoundedRect(10, 0, width() - 10, height(), 10, 10, Qt::AbsoluteSize);
            QColor darkColor(0x98, 0xAF, 0x11);
            /*if (!isEnabled()) {
                darkColor = QColor(0x9f9f9f);
            }*/
            // draw it
            painter.fillPath(darkPath, QBrush(darkColor));

            // light path
            QPainterPath lightPath;
            // arrow
            lightPath.moveTo(14, 10);
            lightPath.quadTo(14, 18, 6, 24);
            lightPath.quadTo(14, 30, 14, 38);
            lightPath.lineTo(14, 10);
            // area
            lightPath.addRoundedRect(14, 4, width() - 18, height() - 8, 7, 7, Qt::AbsoluteSize);
            QColor lightColor(0xd2ff00);
            /*if (!isEnabled()) {
                lightColor = QColor(0xe9e9e9);
            }*/

            // draw it
            painter.fillPath(lightPath, QBrush(lightColor));

            painter.setRenderHint(QPainter::Antialiasing, false);

            QWidget::paintEvent(e);

            painter.setRenderHint(QPainter::Antialiasing, true);

            QLinearGradient gradient(0, 0, 0, height());
            gradient.setColorAt(0.0, QColor(255, 255, 255, 128));
            gradient.setColorAt(0.85, Qt::transparent);
            gradient.setColorAt(1.0, QColor(0, 0, 0, 48));
            painter.fillPath(lightPath, QBrush(gradient));

            painter.setRenderHint(QPainter::Antialiasing, false);
        }

    }
}

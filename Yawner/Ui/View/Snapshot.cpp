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

#include "Snapshot.h"
#include <QPixmap>
#include <QPainter>
#include <QLayout>
#include <QRgb>

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {

            Snapshot::Snapshot(QWidget *target, QWidget *alternative, QWidget *parent) :
                QWidget(parent),
                _target(target),
                _direction(Left),
                _progress(0.f),
                _disposed(false)
            {
                if (alternative == 0) {
                    alternative = _target;
                }
                QPixmap viewDump(alternative->size());
                alternative->render(&viewDump);
                _image = viewDump.toImage();
                setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                setAttribute(Qt::WA_TransparentForMouseEvents, true);
            }

            void Snapshot::paintEvent(QPaintEvent *event)
            {
                QPainter painter(this);

                QImage image(_image);
                QColor c;
                int width = this->width()*(1.f - _progress);

                if (width <= 0) {
                    return;
                }

                float s = ((float) width)/_image.width(); // grayness
                int r, g, b, gr, maxX, maxY, x, y, xOff;
                bool right = _direction == Right;
                QRgb pixel;
                xOff = right ? this->width() - width : 0;
                maxY = image.height() < height() ? image.height() : height();
                maxX = image.width() < width ? image.width() : width;
                for (y = 0; y < maxY; ++y) {
                    for (x = 0; x < maxX; ++x) {
                        pixel = _image.pixel(x + xOff, y);
                        b = pixel%0x000100;
                        g = ((pixel-b)%0x010000)/0x000100;
                        r = ((pixel-g-b)%0x01000000)/0x010000;
                        gr = (11*r + 16*g + 5*b)/32; // gray
                        c.setRgb(s*r + (1-s)*gr, s*g + (1-s)*gr, s*b + (1-s)*gr);
                        image.setPixel(x + xOff, y, c.rgb());
                    }
                }

                painter.drawImage(xOff, 0, image, xOff, 0, width);

                int shadowWidth = width/2;
                QLinearGradient gradient(
                    right ? xOff : width,
                    0,
                    right ? xOff + shadowWidth : width - shadowWidth,
                    0
                );
                gradient.setColorAt(0.0, QColor(36, 36, 36, 256*(1-s)));
                gradient.setColorAt(1.0, QColor(36, 36, 36, 0));
                painter.fillRect(
                    right ? xOff : width - shadowWidth,
                    0,
                    shadowWidth,
                    height(),
                    gradient
                );
            }

            float Snapshot::getProgress()
            {
                return _progress;
            }

            void Snapshot::setProgress(float progress)
            {
                _progress = progress;
                repaint();
            }

            void Snapshot::setDirection(Direction direction)
            {
                _direction = direction;
            }

            void Snapshot::dispose()
            {
                if (!_disposed) {
                    parentWidget()->layout()->removeWidget(this);
                    emit disposed(_target);
                    deleteLater();
                    _disposed = true;
                }
            }

        }
    }
}

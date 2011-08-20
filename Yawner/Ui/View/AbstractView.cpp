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

#include "AbstractView.h"
#include <QPainter>
#include <QLinearGradient>

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {

            AbstractView::AbstractView(QWidget *parent) :
                QScrollArea(parent), _isInitialized(false)
            {
            }

            void AbstractView::_init()
            {
                // do something in subclass
            }

            void AbstractView::init()
            {
                if (!_isInitialized) {
                    _init();
                    _isInitialized = true;
                }
            }

            void AbstractView::paintEvent(QPaintEvent *event)
            {
                // This looks nice except for the scrollbar margin
                // Bail out for now
                return;

                QPainter painter(this->viewport());
                QLinearGradient gradient(0, 0, 0, 4);
                gradient.setColorAt(0.0, QColor(36, 36, 36, 128));
                gradient.setColorAt(1.0, QColor(36, 36, 36, 0));
                painter.fillRect(0, 0, width(), 4, gradient);

                QLinearGradient gradient2(0, height() - 4, 0, height());
                gradient2.setColorAt(0.0, QColor(36, 36, 36, 0));
                gradient2.setColorAt(1.0, QColor(36, 36, 36, 128));
                painter.fillRect(0, height() - 4, width(), 4, gradient2);

            }

            void AbstractView::minimized()
            {
                setHidden(true);
            }

            void AbstractView::maximized()
            {
                setMaximumWidth(0xffffff);
                setDisabled(false);
            }

            void AbstractView::beforeShow()
            {
                setHidden(false);
            }

            void AbstractView::beforeHide()
            {
                setDisabled(true);
            }
        }
    }
}

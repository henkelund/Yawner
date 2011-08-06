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

#include "MessageWidget.h"
#include "ui_messagewidget.h"
#include <QListIterator>

namespace YawnerNS {
    namespace UiNS {

        MessageWidget::MessageWidget(YammerNS::Message *message, QWidget *parent) :
            QWidget(parent), _message(message),
            _ui(new Ui::MessageWidget)
        {
            _ui->setupUi(this);
            _message->setParent(this);
            _ui->message->setText(_message->getText());
        }

        MessageWidget::~MessageWidget()
        {
            delete _ui;
        }

    }
}

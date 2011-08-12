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
#include "MessageText.h"
#include "ui_messagewidget.h"
#include <QListIterator>
#include <QScrollBar>
#include "Yammer/User.h"

namespace YawnerNS {
    namespace UiNS {

        MessageWidget::MessageWidget(YammerNS::Message *message, QWidget *parent) :
            QWidget(parent), _message(message),
            _ui(new Ui::MessageWidget)
        {
            _ui->setupUi(this);
            _ui->message->setText(_message->getText());
            _ui->user->layout()->setAlignment(_ui->avatar, Qt::AlignHCenter);
            YammerNS::User* user = _message->getUser();
            if (user != 0) {
                userDataLoaded(user);
            }
            connect(
                _message->getUser(),
                SIGNAL(dataLoaded(YammerNS::Abstract*)),
                this,
                SLOT(userDataLoaded(YammerNS::Abstract*))
            );
        }

        MessageWidget::~MessageWidget()
        {
            delete _ui;
        }

        void MessageWidget::userDataLoaded(YammerNS::Abstract *user)
        {
            _ui->name->setText(_message->getUser()->getData("full_name").toString().replace(QString(" "), QString("\n")));
            _ui->avatar->setPixmap(_message->getUser()->getSmallImage());
        }

    }
}

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

#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include "../../Yammer/Message.h"
#include "../../Yammer/User.h"

namespace Ui {
    class MessageWidget;
}

namespace YawnerNS {
    namespace UiNS {

        class MessageWidget : public QWidget
        {
            Q_OBJECT
        protected:
            YammerNS::Message *_message;

        public:
            explicit MessageWidget(YammerNS::Message *message, QWidget *parent = 0);
            ~MessageWidget();

        private:
            Ui::MessageWidget *_ui;

        public slots:
            void userDataLoaded(YammerNS::Abstract *user);
        };

    }
}

#endif // MESSAGEWIDGET_H

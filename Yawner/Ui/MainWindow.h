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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkRequest>
#include "Yawner.h"
#include "Yammer/Api.h"
#include "Yammer/Message.h"
#include "OAuth/Token.h"
#include "MessageWidget.h"

namespace Ui {
    class MainWindow;
}

namespace YawnerNS {
    namespace UiNS {

        class MainWindow : public QMainWindow
        {
            Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = 0);
            ~MainWindow();

            void showView(QWidget *widget, bool animate = true);

        private:
            Ui::MainWindow *_ui;
            Yawner *_yawner;
            QList<MessageWidget*> _threadWidgets;

        public slots:
            void showThread(int threadId);
            void showUser(int userId);
            void showFeed();
            void showBrowser(QUrl url);
            void fetchMessages();
            void requestTokenRecieved(OAuthNS::Token token);
            void accessTokenRecieved(OAuthNS::Token token);
        };

    }
}

#endif // MAINWINDOW_H

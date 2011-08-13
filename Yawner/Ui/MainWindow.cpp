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

#include "Yawner/Ui/MainWindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QDesktopServices>
#include <QTimer>
#include <QBoxLayout>
#include "Yawner.h"
#include "OAuth/Token.h"
#include "Yammer/Api.h"
#include "Yawner/Ui/MessageWidget.h"

namespace YawnerNS {
    namespace UiNS {

        MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent),
            _ui(new Ui::MainWindow),
            _yawner(Yawner::getInstance())
        {
            _ui->setupUi(this);

            _ui->centralWidget->setStyleSheet(
                QString("QScrollBar { width: 4px; }")
                .append("QScrollBar::up-arrow, QScrollBar::add-line, QScrollBar::down-arrow, QScrollBar::sub-line { background: transparent; border: 0; } ")
                .append("QScrollBar::add-page, QScrollBar::sub-page { background: transparent; } ")
                .append("QScrollBar::handle { border-radius: 2px; border: 0; background: #FF5800; } ")
                .append("QTextBrowser QScrollBar::handle { margin-top: 4px; margin-bottom:4px; } ")
            );

            OAuthNS::Consumer consumer = _yawner->getConsumer();

            while (consumer.isNull()) {
                QInputDialog consumerDialog;
                consumerDialog.setLabelText(QString("Please enter your consumer key.."));
                consumerDialog.exec();

                QInputDialog consumerDialog2;
                consumerDialog2.setLabelText(QString("..and your secret"));
                consumerDialog2.exec();

                consumer = OAuthNS::Consumer(
                    consumerDialog.textValue(),
                    consumerDialog2.textValue()
                );

                _yawner->setConsumer(consumer);
            }

            OAuthNS::Token accessToken = _yawner->getAccessToken();
            if (accessToken.isNull()) {
                _yawner->getYammerApi()->getRequestToken();

                QObject::connect(
                    _yawner->getYammerApi(),
                    SIGNAL(requestTokenRecieved(OAuthNS::Token)),
                    SLOT(requestTokenRecieved(OAuthNS::Token))
                );
            }
            else {
                _yawner->getYammerApi()->setAccessToken(accessToken);
                _yawner->getMessageManager();
                connect(
                    _yawner->getMessageManager(),
                    SIGNAL(newMessagesLoaded(QList<int>)),
                    SLOT(newMessagesLoaded(QList<int>))
                );
                QTimer *timer = new QTimer(this);
                timer->setInterval(60000);
                connect(timer, SIGNAL(timeout()), _yawner->getMessageManager(), SLOT(fetchMessages()));
                timer->start();
            }
        }

        MainWindow::~MainWindow()
        {
            delete _ui;
        }

        void MainWindow::requestTokenRecieved(OAuthNS::Token token)
        {
            QUrl authorizeUrl(QString("https://www.yammer.com/oauth/authorize"));
            authorizeUrl.addQueryItem(QString("oauth_token"), token.getKey());

            if (QDesktopServices::openUrl(authorizeUrl)) {
                QInputDialog pinDialog;
                pinDialog.setLabelText(QString("Please enter pin"));
                pinDialog.exec();
                _yawner->getYammerApi()->getAccessToken(token, pinDialog.textValue());
                QObject::connect(
                    _yawner->getYammerApi(),
                    SIGNAL(accessTokenRecieved(OAuthNS::Token)),
                    SLOT(accessTokenRecieved(OAuthNS::Token))
                );
            }
        }

        void MainWindow::accessTokenRecieved(OAuthNS::Token token)
        {
            if (!token.isNull()) {
                _yawner->setAccessToken(token);
                _yawner->getYammerApi()->setAccessToken(token);
            }
        }

        void MainWindow::newMessagesLoaded(QList<int> messageIds)
        {
            QListIterator<int> it(messageIds);
            it.toBack();
            while (it.hasPrevious()) {
                YawnerNS::UiNS::MessageWidget *mWidget = new YawnerNS::UiNS::MessageWidget(
                _yawner->getMessageManager()->getMessageById(it.previous()), _ui->messageList);
                QVBoxLayout *layout = static_cast<QVBoxLayout*>(_ui->messageList->layout());
                layout->insertWidget(0, mWidget);
            }
            update();
        }

    }
}


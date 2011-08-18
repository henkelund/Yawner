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
#include <QPropertyAnimation>
#include "Yawner.h"
#include "OAuth/Token.h"
#include "Yammer/Api.h"
#include "Yawner/Ui/MessageWidget.h"

namespace YawnerNS {
    namespace UiNS {

        MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent),
            _ui(new Ui::MainWindow),
            _yawner(Yawner::getInstance()),
            _threadWidgets()
        {
            _ui->setupUi(this);
            _ui->feedView->init();
            connect(_ui->feedView, SIGNAL(threadLinkClicked(int)), this, SLOT(showThread(int)));
            connect(_ui->feedView, SIGNAL(userLinkClicked(int)), this, SLOT(showUser(int)));
            connect(_ui->feedView, SIGNAL(webLinkClicked(QUrl)), this, SLOT(showBrowser(QUrl)));
            showView(_ui->feedView, false);
            connect(_ui->threadBackButton, SIGNAL(clicked()), this, SLOT(showFeed()));
            connect(_ui->userBackButton, SIGNAL(clicked()), this, SLOT(showFeed()));

            QFile styleSheet(QString(":/yawner.css"));
            QString css;
            if (styleSheet.open(QFile::ReadOnly)) {
                css = styleSheet.readAll();
                styleSheet.close();
            }

            if (!css.isEmpty()) {
                setStyleSheet(css);
            }

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
                connect(
                    _yawner->getMessageManager(),
                    SIGNAL(newMessagesLoaded(QList<int>)),
                    _ui->feedView,
                    SLOT(newMessagesLoaded(QList<int>))
                );
                QTimer *timer = new QTimer(this);
                timer->setInterval(60000);
                connect(timer, SIGNAL(timeout()), this, SLOT(fetchMessages()));
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

        void MainWindow::fetchMessages()
        {
            statusBar()->showMessage(QString("Fetching messages.."), 3000);
            _yawner->getMessageManager()->requestMessages(YawnerNS::ManagerNS::MessageManager::NewerThan, _ui->feedView->getNewestId());
        }

        void MainWindow::showThread(int threadId)
        {
            QBoxLayout *layout = static_cast<QBoxLayout*>(_ui->threadList->layout());
            while (_threadWidgets.count() > 0) {
                YawnerNS::UiNS::MessageWidget *mWidget = _threadWidgets.takeAt(0);
                layout->removeWidget(mWidget);
                delete mWidget;
            }

            QList<YammerNS::Message*> messages = _yawner->getMessageManager()->getThreadMessages(threadId);
            QListIterator<YammerNS::Message*> it(messages);
            while (it.hasNext()) {
                YammerNS::Message *message = it.next();
                YawnerNS::UiNS::MessageWidget *mWidget = new YawnerNS::UiNS::MessageWidget(message, _ui->threadList);
                _threadWidgets.append(mWidget);
                layout->addWidget(mWidget);
            }
            showView(_ui->threadView);
        }

        void MainWindow::showUser(int userId)
        {
            showView(_ui->userView);
        }

        void MainWindow::showBrowser(QUrl url)
        {
            QDesktopServices::openUrl(url);
        }

        void MainWindow::showFeed()
        {
            showView(_ui->feedView);
        }

        void MainWindow::showView(QWidget *widget, bool animate)
        {
            QListIterator<QObject*> it(static_cast<QList<QObject*> >(_ui->bodyWidget->children()));

            while (it.hasNext()) {
                QObject *nextObject = it.next();
                // this kind of casting and type checking feels ugly
                if (!nextObject->isWidgetType()) {
                    continue;
                }

                YawnerNS::UiNS::ViewNS::AbstractView *view = static_cast<YawnerNS::UiNS::ViewNS::AbstractView*>(nextObject);

                QPropertyAnimation *animation = 0;
                if (animate) {
                    animation = new QPropertyAnimation(view, "maximumWidth");
                    animation->setDuration(500);
                    animation->setEasingCurve(QEasingCurve::InOutCubic);
                }

                if (view == widget) {
                    view->beforeShow();
                    view->setHidden(false);
                    if (animate) {
                        animation->setStartValue(view->width());
                        animation->setEndValue(width());
                        connect(animation, SIGNAL(finished()), view, SLOT(maximized()));
                    }
                    else {
                        view->maximized();
                    }
                }
                else {
                    view->beforeHide();
                    view->setDisabled(true);
                    if (animate) {
                        animation->setStartValue(width());
                        animation->setEndValue(0);
                        connect(animation, SIGNAL(finished()), view, SLOT(minimized()));
                    }
                    else {
                        view->minimized();
                    }
                }
                if (animate) {
                    animation->start(QAbstractAnimation::DeleteWhenStopped);
                }
            }
        }
    }
}

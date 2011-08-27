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

#include "FeedView.h"
#include "Yawner.h"

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {

            FeedView::FeedView(QWidget *parent) :
                AbstractView(parent),
                _messageLayout(0),
                _messagesWidgets(),
                _hasChanges(false),
                _showOlderButton(0)
            {
            }

            void FeedView::_init()
            {
                _showOlderButton = findChild<QPushButton*>("showOlderButton");
                connect(_showOlderButton, SIGNAL(clicked()), this, SLOT(showOlder()));

                QList<int> messageIds = Yawner::getInstance()->getMessageManager()->getLastLoadIds();
                if (messageIds.count() > 0) {
                    newMessagesLoaded(messageIds);
                }
            }

            QBoxLayout* FeedView::_getMessageLayout()
            {
                if (_messageLayout == 0) {
                    QWidget *feedList = viewport()->findChild<QWidget*>("feedList");
                    _messageLayout = static_cast<QBoxLayout*>(feedList->layout());
                }
                return _messageLayout;
            }

            void FeedView::_syncLayout()
            {
                if (!_hasChanges) {
                    return;
                }
                QListIterator<YawnerNS::UiNS::MessageWidget*> all(_messagesWidgets);
                QBoxLayout *layout = _getMessageLayout();
                QListIterator<YawnerNS::UiNS::MessageWidget*> old(layout->findChildren<YawnerNS::UiNS::MessageWidget*>());

                while (all.hasNext()) {
                    YawnerNS::UiNS::MessageWidget *widget = all.next();
                    bool inserted = false;
                    old.toFront();
                    while (old.hasNext()) {
                        YawnerNS::UiNS::MessageWidget *oldWidget = old.next();
                        if (widget->isTimestampGreaterThan(oldWidget)) {
                            layout->insertWidget(layout->indexOf(oldWidget), widget);
                            inserted = true;
                            break;
                        }
                    }
                    if (!inserted) {
                        layout->addWidget(widget);
                    }
                }
                _hasChanges = false;
            }

            void FeedView::beforeShow()
            {
                _syncLayout();
                AbstractView::beforeShow();
            }

            void FeedView::newMessagesLoaded(QList<int> messageIds)
            {
                int i = 0;
                QListIterator<int> it(messageIds);
                while (it.hasNext()) {
                    YammerNS::Message *message = Yawner::getInstance()->getMessageManager()->getMessageById(it.next());
                    YawnerNS::UiNS::MessageWidget *widget = new YawnerNS::UiNS::MessageWidget(message, this);
                    connect(widget, SIGNAL(threadLinkClicked(int)), this, SLOT(messageThreadLinkClicked(int)));
                    connect(widget, SIGNAL(userLinkClicked(int)), this, SLOT(messageUserLinkClicked(int)));
                    connect(widget, SIGNAL(webLinkClicked(QUrl)), this, SLOT(messageWebLinkClicked(QUrl)));
                    if (0 == i++ && message->getId() > getNewestId()) {
                        Yawner::getInstance()->getNotificationManager()->show(message);
                    }
                    _messagesWidgets.append(widget);
                }
                qSort(
                    _messagesWidgets.begin(),
                    _messagesWidgets.end(),
                    YawnerNS::UiNS::message_widget_timestamp_greater_than
                );
                _hasChanges = true;
                if (!isHidden()) {
                    _syncLayout();
                }
            }

            void FeedView::showOlder()
            {
                if (_messagesWidgets.isEmpty()) {
                    return;
                }
                int id = getOldestId();
                if (id > 0) {
                    Yawner::getInstance()
                        ->getMessageManager()
                        ->requestMessages(
                            YawnerNS::ManagerNS::MessageManager::OlderThan,
                            id
                        );
                }
            }

            int FeedView::getNewestId()
            {
                if (_messagesWidgets.count() == 0) {
                    return -1;
                }
                return _messagesWidgets.first()->getMessage()->getId();
            }

            int FeedView::getOldestId()
            {
                if (_messagesWidgets.count() == 0) {
                    return -1;
                }
                return _messagesWidgets.last()->getMessage()->getId();
            }

            void FeedView::messageThreadLinkClicked(int threadId)
            {
                emit threadLinkClicked(threadId);
            }

            void FeedView::messageUserLinkClicked(int userId)
            {
                emit userLinkClicked(userId);
            }

            void FeedView::messageWebLinkClicked(QUrl url)
            {
                webLinkClicked(url);
            }
        }
    }
}

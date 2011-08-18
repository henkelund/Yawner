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

#ifndef FEEDVIEW_H
#define FEEDVIEW_H

#include "AbstractView.h"
#include <QBoxLayout>
#include <QPushButton>
#include "../MessageWidget.h"

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {

            class FeedView : public AbstractView
            {
                Q_OBJECT
            private:
                QBoxLayout *_messageLayout;
                QList<YawnerNS::UiNS::MessageWidget*> _messagesWidgets;
                bool _hasChanges;
                QPushButton *_showOlderButton;

                QBoxLayout* _getMessageLayout();
                void _syncLayout();

            protected:
                void _init();

            public:
                explicit FeedView(QWidget *parent = 0);

                void beforeShow();

                int getNewestId();

                int getOldestId();

            signals:
                void threadLinkClicked(int threadId);
                void userLinkClicked(int userId);
                void webLinkClicked(QUrl url);

            public slots:
                void newMessagesLoaded(QList<int> messageIds);
                void showOlder();
                void messageThreadLinkClicked(int threadId);
                void messageUserLinkClicked(int userId);
                void messageWebLinkClicked(QUrl url);
            };

        }
    }
}

#endif // FEEDVIEW_H

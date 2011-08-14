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
#include <QTimer>
#include <QPainter>
#include <QLinearGradient>
#include <QDesktopServices>
#include <QMessageBox>
#include "Yammer/User.h"
#include "Yawner.h"

namespace YawnerNS {
    namespace UiNS {

        MessageWidget::MessageWidget(YammerNS::Message *message, QWidget *parent) :
            QWidget(parent), _message(message),
            _ui(new Ui::MessageWidget)
        {
            _ui->setupUi(this);
            _ui->user->layout()->setAlignment(_ui->avatar, Qt::AlignHCenter);
            _ui->message->setOpenLinks(false);
            _ui->message->setOpenExternalLinks(false);

            connect(_ui->message, SIGNAL(anchorClicked(QUrl)), this, SLOT(anchorClicked(QUrl)));

            YammerNS::User* user = _message->getUser();
            if (user != 0) {
                userDataLoaded(user);
            }
            connect(
                _message,
                SIGNAL(dataLoaded(YammerNS::Abstract*)),
                this,
                SLOT(messageDataLoaded(YammerNS::Abstract*))
            );
            connect(
                _message->getUser(),
                SIGNAL(dataLoaded(YammerNS::Abstract*)),
                this,
                SLOT(userDataLoaded(YammerNS::Abstract*))
            );
            messageDataLoaded(_message);
        }

        MessageWidget::~MessageWidget()
        {
            delete _ui;
        }

        void MessageWidget::processMessageData()
        {
            _ui->name->setText(_message->getUser()->getData("full_name").toString().replace(QString(" "), QString("\n")));

            QPixmap avatar = _message->getUser()->getSmallImage();
            _ui->avatar->setPixmap(_decorateAvatar(&avatar));

            QString repliedToName;
            YammerNS::Message *repliedToMessage = 0;
            int repliedToId = _message->getData("replied_to_id").toInt();
            if (repliedToId > 0) {
                repliedToMessage = Yawner::getInstance()->getMessageManager()->getMessageById(repliedToId);
                repliedToName = repliedToMessage->getUser()->getData("full_name").toString();
            }

            _ui->message->setHtml(_filterText(_message->getText()));
            update();
        }

        void MessageWidget::messageDataLoaded(YammerNS::Abstract *message)
        {
            QTimer::singleShot(100, this, SLOT(processMessageData()));
        }

        void MessageWidget::userDataLoaded(YammerNS::Abstract *user)
        {
            QTimer::singleShot(100, this, SLOT(processMessageData()));
        }

        void MessageWidget::paintEvent(QPaintEvent *e)
        {
            QPainter painter(this);

            QLinearGradient gradient(0, 0, 0, height());
            gradient.setColorAt(0.0, QColor(0, 0, 0, 32));
            gradient.setColorAt(0.20, Qt::transparent);
            gradient.setColorAt(0.80, Qt::transparent);
            gradient.setColorAt(1.0, QColor(255, 255, 255, 24));

            QPainterPath roundRect;
            roundRect.addRoundedRect(0, 0, width(), height(), 16, 16, Qt::AbsoluteSize);

            painter.fillPath(roundRect, gradient);
        }

        void MessageWidget::anchorClicked(QUrl url)
        {
            QRegExp threadPattern("thread:([0-9]+)");
            if (QRegExp("^https?:\\/\\/").indexIn(url.toString()) != -1) {
                QDesktopServices::openUrl(url);
            }
            else if (threadPattern.indexIn(url.toString()) != -1) {
                int threadId = threadPattern.cap(1).toInt();
                if (threadId > 0) {
                    QMessageBox msg(this);
                    msg.setText(
                        QString("Thread view is not implemented yet.")
                    );
                    msg.setIcon(QMessageBox::Information);
                    msg.exec();
                }
            }
        }

        QPixmap MessageWidget::_decorateAvatar(QPixmap *avatar)
        {
            QBrush brush(*avatar);
            QPixmap roundedAvatar(avatar->width(), avatar->height());
            roundedAvatar.fill(Qt::transparent);
            QPainter painter(&roundedAvatar);
            QPainterPath roundRect;
            roundRect.addRoundedRect(0, 0, roundedAvatar.width(), roundedAvatar.height(), 10, 10, Qt::AbsoluteSize);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.fillPath(roundRect, brush);
            painter.setRenderHint(QPainter::Antialiasing, false);
            return roundedAvatar;
        }

        QString MessageWidget::_filterText(QString rawText)
        {
            QRegExp linkPattern("(https?:\\/\\/[^\\s]+[\\w\\/])");
            QStringList linkMatches;
            int pos = 0;

            while ((pos = linkPattern.indexIn(rawText, pos)) != -1) {
                linkMatches.append(linkPattern.cap(1));
                pos += linkPattern.matchedLength();
            }

            QStringListIterator linkIt(linkMatches);
            while (linkIt.hasNext()) {
                QString link(linkIt.next());
                rawText = rawText.replace(
                    link,
                    QString("<a style=\"color: #008b9e; font: 700 10px; text-decoration: none;\" href=\"%1\">%1</a>").arg(link)
                 );
            }

            QRegExp userPattern("\\[\\[user:([0-9]+)\\]\\]");
            QStringList userMatches;
            pos = 0;

            while ((pos = userPattern.indexIn(rawText, pos)) != -1) {
                userMatches.append(userPattern.cap(1));
                pos += userPattern.matchedLength();
            }

            QStringListIterator it(userMatches);
            while (it.hasNext()) {
                int uid = it.next().toInt();
                if (uid > 0) {
                    YammerNS::User *user = Yawner::getInstance()->getUserManager()->getUserById(uid);
                    rawText = rawText.replace(
                        QString("[[user:%1]]").arg(uid),
                        QString("<a style=\"color: #008b9e; font: 700 10px; text-decoration: none;\" href=\"#\">%1</a>").arg(user->getData("full_name").toString())
                     );
                }
            }

            QString repliedToName;
            YammerNS::Message *repliedToMessage = 0;
            int repliedToId = _message->getData("replied_to_id").toInt();
            if (repliedToId > 0) {
                repliedToMessage = Yawner::getInstance()->getMessageManager()->getMessageById(repliedToId);
                repliedToName = repliedToMessage->getUser()->getData("full_name").toString();
            }

            if (!repliedToName.isEmpty()) {
                rawText.prepend(QString("@<a style=\"color: #FF5800; font: 700 10px; text-decoration: none;\" href=\"thread:%1\">%2</a>: ").arg(QString::number(repliedToId), repliedToName));
            }

            return rawText.replace(QString("\n"), QString("<br />"));
        }
    }
}

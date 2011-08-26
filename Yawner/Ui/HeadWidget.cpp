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

#include "HeadWidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QMessageBox>
#include <QLayout>
#include "Yawner.h"
#include "Yammer/User.h"

namespace YawnerNS {
    namespace UiNS {

        HeadWidget::HeadWidget(QWidget *parent) :
            QWidget(parent), _textEdit(0)
        {
        }

        void HeadWidget::init()
        {
            _textEdit = findChild<YawnerNS::UiNS::HeadWidgetNS::PostTextEdit*>("postMulti");
            _textEdit->setTabChangesFocus(true);
            layout()->setAlignment(_textEdit, Qt::AlignTop);
            _submitButton = findChild<QPushButton*>("postSubmit");
            connect(_submitButton, SIGNAL(clicked()), this, SLOT(submitClicked()));
        }

        void HeadWidget::submitClicked()
        {
            if (!_textEdit->isEmpty()) {
                Yawner::getInstance()
                        ->getMessageManager()
                        ->postMessage(_textEdit->toPlainText());
                _textEdit->clear();
                _textEdit->clearFocus();
            }
            else {
                QMessageBox dialog(this);
                dialog.setWindowTitle("Error sending message");
                dialog.setIconPixmap(QPixmap(QString(":/icon48.png")));
                dialog.setText("Cannot post empty message.");
                dialog.exec();
            }
        }

        void HeadWidget::paintEvent(QPaintEvent *event)
        {
            QWidget::paintEvent(event);

            // needed to apply stylesheet
            QStyleOption option;
            option.init(this);
            QPainter painter(this);
            style()->drawPrimitive(
                QStyle::PE_Widget,
                &option,
                &painter,
                this
            );
        }

    }
}

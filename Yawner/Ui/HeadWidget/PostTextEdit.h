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
* @category Yawner/Ui/HeadWidget
* @package Yawner/Ui/HeadWidget
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link http://yawner.henkehedelund.se/
*/

#ifndef POSTTEXTEDIT_H
#define POSTTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QCompleter>

namespace YawnerNS {
    namespace UiNS {
        namespace HeadWidgetNS {

            class PostTextEdit : public QTextEdit
            {
                Q_OBJECT
            private:
                QCompleter              *_mentionCompleter;
                static const QString    _placeholder;
                static const QString    _placeholderWrapper;

                void setHeightInLines(int lines, bool animate = false);

            protected:
                void keyPressEvent(QKeyEvent *event);
                void focusInEvent(QFocusEvent *event);
                void focusOutEvent(QFocusEvent *event);

            public:
                explicit PostTextEdit(QWidget *parent = 0);

                bool isEmpty();

                virtual void clear();

            public slots:
                void insertCompletion(QString completion);
                void newUsersLoaded(QList<int> newUserIds);
            };

        }
    }
}

#endif // POSTTEXTEDIT_H

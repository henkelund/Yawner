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

#include "PostTextEdit.h"
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QPropertyAnimation>
#include <QStringListModel>
#include "Yawner.h"
#include "Yammer/User.h"

namespace YawnerNS {
    namespace UiNS {
        namespace HeadWidgetNS {

            const QString PostTextEdit::_placeholder = QString("What are you \"working\" on?");
            const QString PostTextEdit::_placeholderWrapper = QString("<i>%1</i>");

            PostTextEdit::PostTextEdit(QWidget *parent) :
                QTextEdit(parent), _mentionCompleter(new QCompleter(this))
            {
                _mentionCompleter->setWidget(this);
                _mentionCompleter->setCompletionMode(QCompleter::PopupCompletion);
                _mentionCompleter->setCaseSensitivity(Qt::CaseInsensitive);
                connect(
                    _mentionCompleter,
                    SIGNAL(activated(QString)),
                    this,
                    SLOT(insertCompletion(QString))
                );
                clearFocus();
            }

            void PostTextEdit::newUsersLoaded(QList<int> newUserIds)
            {
                Q_UNUSED(newUserIds);

                QMap<int, YammerNS::User*> userIndex =
                        Yawner::getInstance()->getUserManager()->getUserIndex();

                QStringList userNames;

                QMapIterator<int, YammerNS::User*> it(userIndex);
                while (it.hasNext()) {
                    YammerNS::User *user = it.next().value();
                    if (user->isLoaded()) {
                        userNames.append(user->getUserName());
                    }
                }

                QStringListModel *dict = new QStringListModel(userNames, _mentionCompleter);
                _mentionCompleter->setModel(dict);
            }

            void PostTextEdit::insertCompletion(QString completion)
            {
                QTextCursor tc = textCursor();
                int extra = completion.length() - _mentionCompleter->completionPrefix().length();
                tc.movePosition(QTextCursor::Left);
                tc.movePosition(QTextCursor::EndOfWord);
                tc.insertText(completion.right(extra));
                setTextCursor(tc);
            }

            void PostTextEdit::setHeightInLines(int lines, bool animate)
            {
                qreal innerHeight = document()->size().height() - (document()->documentMargin()*2);
                qreal lineHeight = innerHeight/document()->lineCount();
                qreal finalHeight = (lines*lineHeight) + (document()->documentMargin()*2);
                if (animate) {
                    QPropertyAnimation *animation = new QPropertyAnimation();
                    animation->setTargetObject(this);
                    animation->setPropertyName("minimumHeight");
                    animation->setDuration(250);
                    animation->setStartValue(minimumHeight());
                    animation->setEndValue(finalHeight + 10); // not sure where the "10" comes from but it looks ok
                    animation->start(QAbstractAnimation::DeleteWhenStopped);
                }
                else {
                    setMinimumHeight(finalHeight + 10); // not sure where the "10" comes from but it looks ok
                }
            }

            bool PostTextEdit::isEmpty()
            {
                return toPlainText().trimmed().isEmpty() ||
                        toPlainText().trimmed() == _placeholder.trimmed();
            }

            void PostTextEdit::clear()
            {
                QTextEdit::clear();
                setText(_placeholderWrapper.arg(_placeholder));
            }

            void PostTextEdit::keyPressEvent(QKeyEvent *event)
            {
                if (_mentionCompleter->popup()->isVisible()) {
                    // The following keys are forwarded by the completer to the widget
                    switch (event->key()) {
                        case Qt::Key_Enter:
                        case Qt::Key_Return:
                        case Qt::Key_Escape:
                        case Qt::Key_Tab:
                        case Qt::Key_Backtab:
                            event->ignore();
                            return; // let the completer do default behavior
                        default:
                        break;
                    }
                }
                QTextEdit::keyPressEvent(event);
                setHeightInLines(document()->lineCount());

                if (event->text().isEmpty()) {
                    return;
                }

                QString text = toPlainText();
                int cursorPos = textCursor().position();
                QString leftOfCursor = text.left(cursorPos);
                QRegExp mentionPattern("(^|\\s)@([a-zA-Z0-9]*)$");

                if (mentionPattern.indexIn(leftOfCursor) >= 0) {
                    QString mentionAttempt = mentionPattern.cap(2);
                    _mentionCompleter->setCompletionPrefix(mentionAttempt);
                    _mentionCompleter->popup()->setCurrentIndex(
                        _mentionCompleter->completionModel()->index(0, 0)
                    );
                    _mentionCompleter->complete();
                }
                else {
                    _mentionCompleter->popup()->hide();
                }
            }

            void PostTextEdit::focusInEvent(QFocusEvent *event)
            {
                QTextEdit::focusInEvent(event);
                setLineWrapMode(WidgetWidth);
                if (toPlainText().trimmed() == _placeholder.trimmed()) {
                    setText("");
                }
                setHeightInLines(document()->lineCount(), true);
            }

            void PostTextEdit::focusOutEvent(QFocusEvent *event)
            {
                QTextEdit::focusOutEvent(event);
                setLineWrapMode(NoWrap);
                if (toPlainText().trimmed().length() == 0) {
                    clear();
                }
                setHeightInLines(1, true);
            }

        }
    }
}

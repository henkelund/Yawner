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
* @category Yammer
* @package Yammer
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link http://yawner.henkehedelund.se/
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QScriptValue>
#include "Abstract.h"
#include "User.h"

namespace YammerNS {

    class Message : public Abstract
    {
        Q_OBJECT
    private:
        long _timestamp;

    protected:
        virtual bool _beforeLoad(QVariantMap *data);

    public:
        explicit Message(QObject *parent = 0);

        explicit Message(int id, QObject *parent = 0);

        int getRepliedToId();

        int getThreadStarterId();

        QString getText();

        QString getExcerpt();

        User* getUser();

        long getTimestamp();

        bool isNewerThan(YammerNS::Message *other);

        bool isOlderThan(YammerNS::Message *other);

    signals:

    public slots:

    };

    bool message_is_newer_than(YammerNS::Message *left, YammerNS::Message *right);
    bool message_is_older_than(YammerNS::Message *left, YammerNS::Message *right);

}

#endif // MESSAGE_H

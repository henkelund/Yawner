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

#include "Message.h"
#include <QDateTime>
#include "Yawner.h"

namespace YammerNS {

    Message::Message(QObject *parent) :
        Abstract(parent), _timestamp(0)
    {
    }

    bool Message::_beforeLoad(QVariantMap *data)
    {
        if (data->value(QString("id")).toInt() <= 0) {
            qDebug(QString("No message ID").toStdString().c_str());
            return false;
        }
        return true;
    }

    int Message::getId()
    {
        return getData(QString("id")).toInt();
    }

    int Message::getParentId()
    {
        return getData(QString("replied_to_id")).toInt();
    }

    QString Message::getText()
    {
        return getData(QString("body")).toMap().value(QString("parsed")).toString();
    }

    User* Message::getUser()
    {
        return
            Yawner::getInstance()
                ->getUserManager()
                ->getUserById(
                    getData(QString("sender_id")).toInt()
                );
    }

    long Message::getTimestamp()
    {
        if (_timestamp == 0) {
            QDateTime time = QDateTime::fromString(
                        getData("created_at").toString(),
                        QString("yyyy/MM/dd hh:mm:ss +0000")
            );
            if (time.isValid()) {
                _timestamp = time.toTime_t();
            }
        }
        return _timestamp;
    }

}

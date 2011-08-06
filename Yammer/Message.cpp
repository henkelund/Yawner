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

namespace YammerNS {

    Message::Message(QObject *parent) :
        QObject(parent), _id(0), _parentId(0), _text("")
    {
    }

    int Message::getId()
    {
        return _id;
    }

    int Message::getParentId()
    {
        return _parentId;
    }

    void Message::setText(QString text)
    {
        _text = text;
    }

    QString Message::getText()
    {
        return _text;
    }

    bool Message::isComment()
    {
        return _parentId != 0;
    }

    Message* Message::fromScriptValue(QScriptValue value, QObject *parent)
    {
        Message *message = new Message(parent);
        message->_id = value.property("id").toInteger();
        message->_parentId = value.property("replied_to_id").toInteger();
        message->_text = value.property("body").property("plain").toString();
        return message;
    }

}

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
* @link https://github.com/henkelund/Yawner
*/

#include "Abstract.h"
#include <QScriptEngine>

namespace YammerNS {

    Abstract::Abstract(QObject *parent) :
        QObject(parent)
    {
    }

    bool Abstract::_beforeLoad(QVariantMap *data)
    {
        // do nothing but give subclasses a chance to do something
        return true;
    }

    bool Abstract::_afterLoad(QVariantMap *data)
    {
        // do nothing but give subclasses a chance to do something
        return true;
    }

    bool Abstract::load(QVariantMap data)
    {
        bool success = false;
        if (success = _beforeLoad(&data)) {
            _data = data;
        }
        if (success = (success && _afterLoad(&_data))) {
            emit dataLoaded(this);
        }
        return success;
    }

    QVariant Abstract::getData(QString key)
    {
        return _data.value(key);
    }

    bool Abstract::loadJson(Abstract* obj, QString json)
    {
        if (obj == 0)
            return false;

        QScriptEngine engine;
        QScriptValue jObj = engine.evaluate(QString("(").append(json).append(")"));
        if (!jObj.isObject())
            return false;

        QVariant data = jObj.toVariant();
        if (!data.isValid())
            return false;

        return obj->load(data.toMap());
    }

}

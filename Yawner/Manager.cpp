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
* @category Yawner
* @package Yawner
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link http://yawner.henkehedelund.se/
*/

#include "Manager.h"
#include <QScriptEngine>
#include <QScriptValue>
#include "../Yawner.h"

namespace YawnerNS {

    Manager::Manager(QObject *parent) :
        QObject(parent), _isInitialized(false)
    {
    }

    void Manager::_init()
    {

    }

    void Manager::init()
    {
        if (!_isInitialized) {
            _init();
            _isInitialized = true;
        }
    }

    Yawner* Manager::_yawner()
    {
        return Yawner::getInstance();
    }

    YammerNS::Api* Manager::_api()
    {
        return _yawner()->getYammerApi();
    }

    void Manager::_storeObjectList(QString name, QList<YammerNS::Abstract*> list)
    {
        QScriptEngine engine;
        QStringList jsonBody;

        for (int i = 0; i < list.length(); ++i) {
            QVariant objectData = list.at(i)->getData();
            QScriptValueList args;
            args.append(engine.toScriptValue(objectData));
            jsonBody.append(engine.evaluate("JSON.stringify").call(engine.globalObject(), args).toString());
        }

        _yawner()->putFileContents(name.append(".json"), QString("[%1]").arg(jsonBody.join(",")));
    }

    QVariantList Manager::_loadStoredObjectList(QString name)
    {
        QScriptEngine engine;
        QVariant objectData = engine.evaluate(
                QString("(%1)").arg(
                    _yawner()->getFileContents(name.append(".json")))
                ).toVariant();

        if (objectData.isValid() && objectData.canConvert(QVariant::List)) {
            return objectData.toList();
        }
        return QVariantList();
    }
}

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
* @category OAuth
* @package OAuth
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/lgpl.html GNU GPL
* @link https://github.com/henkelund/Yawner
*/

#include "Util.h"
#include <QStringList>
#include <QUrl>

namespace OAuthNS {

    Util::Util()
    {
    }

    /* static */
    QMap<QString, QString> Util::parseParameters(QString parameters)
    {
        QStringList pairs = parameters.split(QString("&"));
        QMap<QString, QString> result;
        for (int i = 0; i < pairs.length(); ++i) {
            QStringList keyVal = pairs.at(i).split("=");
            if (keyVal.length() < 2) {
                continue;
            }
            result.insert(
                QUrl::fromPercentEncoding(keyVal.at(0).toUtf8()),
                QUrl::fromPercentEncoding(keyVal.at(1).toUtf8())
            );
        }
        return result;
    }

    /* static */
    QString Util::buildParameterString(QMap<QString, QString> parameters)
    {
        QStringList pairs;
        QStringList keys = parameters.keys();
        keys.sort();
        for (int i = 0; i < keys.length(); ++i) {
            pairs.append(
                QUrl::toPercentEncoding(keys.at(i))
                    .append("=")
                    .append(QUrl::toPercentEncoding(parameters.value(keys.at(i))))
            );
        }
        return pairs.join("&");
    }
}

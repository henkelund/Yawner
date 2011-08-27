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

#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include "../Yammer/Api.h"

class Yawner;

namespace YawnerNS {

    class Manager : public QObject
    {
        Q_OBJECT
    protected:

        bool _isInitialized;

        virtual void _init();

        YammerNS::Api* _api();

        Yawner* _yawner();

        void _storeObjectList(QString name, QList<YammerNS::Abstract*> list);

        QVariantList _loadStoredObjectList(QString name);

    public:
        explicit Manager(QObject *parent = 0);

        void init();

    signals:

    public slots:

    };

}

#endif // MANAGER_H

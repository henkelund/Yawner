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

#ifndef ABSTRACT_H
#define ABSTRACT_H

#include <QObject>
#include <QVariantMap>

namespace YammerNS {

    class Abstract : public QObject
    {
        Q_OBJECT
    protected:
        int         _id;
        QString     _idKey;
        QVariantMap _data;
        bool        _isLoaded;

        virtual bool _beforeLoad(QVariantMap *data);
        virtual bool _afterLoad(QVariantMap *data);

    public:
        explicit Abstract(QObject *parent = 0);

        explicit Abstract(int id, QObject *parent = 0);

        virtual int getId();

        bool load(QVariantMap data, bool *isFirstLoad = 0);

        bool isLoaded();

        QVariant getData(QString key = QString());

    signals:
        void dataLoaded(YammerNS::Abstract *obj);

    public slots:

    };

}

#endif // ABSTRACT_H

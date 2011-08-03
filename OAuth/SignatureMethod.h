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

#ifndef SIGNATUREMETHOD_H
#define SIGNATUREMETHOD_H

#include <QObject>
#include "Consumer.h"
#include "Token.h"

namespace OAuthNS {

    // Can't #include "Request.h" due to mutual dependency
    // Pre declare class here instead
    class Request;

    class SignatureMethod : public QObject
    {
        Q_OBJECT
    public:
        explicit SignatureMethod(QObject *parent = 0);

        /**
         *
         * @return QString The name of this signature
         */
        virtual QString getName() = 0;

        /**
         *
         * @param Request request
         * @param Consumer consumer
         * @param Token token
         * @return QString
         */
        virtual QString buildSignature(Request *request, Consumer consumer, Token token) = 0;

        /**
         *
         * @param Request request
         * @param Consumer consumer
         * @param Token token
         * @param QString signature
         * @return bool
         */
        bool checkSignature(Request *request, Consumer consumer, Token token, QString signature);

    signals:

    public slots:

    };

}

#endif // SIGNATUREMETHOD_H

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
* @category OAuth/SignatureMethod
* @package OAuth/SignatureMethod
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/lgpl.html GNU GPL
* @link https://github.com/henkelund/Yawner
*/

#ifndef PLAINTEXT_H
#define PLAINTEXT_H

#include "../SignatureMethod.h"
#include "../Request.h"

namespace OAuthNS {

    namespace SignatureMethodNS {

        class Plaintext : public SignatureMethod
        {
            Q_OBJECT
        public:
            explicit Plaintext(QObject *parent = 0);

            /**
             *
             * @return QString The name of this signature
             */
            QString getName();

            /**
             *
             * @param Request request
             * @param Consumer consumer
             * @param Token token
             * @return QString
             */
            QString buildSignature(Request *request, Consumer consumer, Token token);

        signals:

        public slots:

        };

    }

}

#endif // PLAINTEXT_H

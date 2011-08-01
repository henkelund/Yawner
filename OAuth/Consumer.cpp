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
* @link http://yawner.henkehedelund.se/
*/

#include "Consumer.h"

namespace OAuthNS {

    /**
     *
     * @param const QString key
     * @param const QString secret
     * @param const QObject *parent
     */
    Consumer::Consumer(const QString key, const QString secret) :
        _key(key), _secret(secret), _callbackUrl("")
    {
    }

    /**
     *
     * @param Consumer &consumer
     */
    Consumer::Consumer(const Consumer &consumer)
    {
        _key = consumer._key;
        _secret = consumer._secret;
        _callbackUrl = consumer._callbackUrl;
    }

    /**
     *
     * @return QString
     */
    QString Consumer::getKey()
    {
        return _key;
    }

    /**
     *
     * @return QString
     */
    QString Consumer::getSecret()
    {
        return _secret;
    }

}

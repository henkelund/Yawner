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
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link https://github.com/henkelund/Yawner
*/

#include "Token.h"
#include <QUrl>

namespace OAuthNS {

    Token::Token() :
        _key(), _secret()
    {
    }

    Token::Token(const QString key, const QString secret) :
        _key(key), _secret(secret)
    {
    }

    Token::Token(const Token &token)
    {
        _key = token._key;
        _secret = token._secret;
    }

    QString Token::getKey()
    {
        return _key;
    }

    QString Token::getSecret()
    {
        return _secret;
    }

    QString Token::toParamString()
    {
        return
            QString("oauth_token=")
                .append(QUrl::toPercentEncoding(_key))
                .append("&oauth_token_secret=")
                .append(QUrl::toPercentEncoding(_secret));
    }

    bool Token::isNull()
    {
        return _key.isEmpty() || _secret.isEmpty();
    }

}

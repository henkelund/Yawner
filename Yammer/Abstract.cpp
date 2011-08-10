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

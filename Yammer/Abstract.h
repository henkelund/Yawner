#ifndef ABSTRACT_H
#define ABSTRACT_H

#include <QObject>
#include <QVariantMap>

namespace YammerNS {

    class Abstract : public QObject
    {
        Q_OBJECT
    protected:
        QVariantMap _data;

        virtual bool _beforeLoad(QVariantMap *data);
        virtual bool _afterLoad(QVariantMap *data);

    public:
        explicit Abstract(QObject *parent = 0);

        bool load(QVariantMap data);

        QVariant getData(QString key);

        static bool loadJson(Abstract* obj, QString json);

    signals:
        void dataLoaded(YammerNS::Abstract *obj);

    public slots:

    };

}

#endif // ABSTRACT_H

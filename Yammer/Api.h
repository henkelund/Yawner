#ifndef API_H
#define API_H

#include <QObject>
#include "../OAuth/Consumer.h"

namespace Yammer {

    class Api : public QObject
    {
        Q_OBJECT
    protected:
        OAuthNS::Consumer _consumer;

    public:
        explicit Api(OAuthNS::Consumer consumer, QObject *parent = 0);

        void getRequestToken();

        void call(const char *method);

    signals:

    public slots:

    };

}

#endif // API_H

#include "Api.h"
#include "../OAuth/Consumer.h"
#include "../OAuth/SignatureMethod/Plaintext.h"
#include "../OAuth/Request.h"

namespace Yammer {

    Api::Api(OAuthNS::Consumer consumer, QObject *parent) :
        _consumer(consumer), QObject(parent)
    {
    }

    void Api::getRequestToken()
    {
        OAuthNS::Request *request = OAuthNS::Request::fromConsumer(
            _consumer,
            QString("https://www.yammer.com/oauth/request_token")
        );

        OAuthNS::SignatureMethodNS::Plaintext sm;
        request->signRequest(&sm, _consumer);
        request->exec();
    }

    void Api::call(const char *method)
    {

    }
}

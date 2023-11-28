#include "HttpClientInterface.h"
#include "AlRightAPI.h"
#include <memory>
using std::make_shared;

#define CONCRETE_REST_NO_WRITE_REQUEST(name) \
    const bool name(URL_AND_HEADERS, RESPONSE) const override { return 0; }
#define CONCRETE_REST_WRITE_REQUEST(name, data) \
    const bool name(URL_AND_HEADERS, data, RESPONSE) const override { return 0; }

class HttpClient : public HttpClientInterface
{
    CONCRETE_REST_NO_WRITE_REQUEST(Head);
    CONCRETE_REST_NO_WRITE_REQUEST(Get);
    CONCRETE_REST_NO_WRITE_REQUEST(Del);
    CONCRETE_REST_WRITE_REQUEST(Post, const std::string &data);
    CONCRETE_REST_WRITE_REQUEST(Put, const std::string &data);
    CONCRETE_REST_WRITE_REQUEST(Put, const ByteBuffer &data);
};

int main()
{
    auto httpClient = make_shared<HttpClient>();
    AlrightAPIClient test(httpClient.get());
}
#include "HttpClientInterface.h"

class AlrightAPIClient
{

public:
    AlrightAPIClient(HttpClientInterface *concreteHttpClient) : httpClient(concreteHttpClient)
    {
    }

private:
    HttpClientInterface *httpClient{};
};
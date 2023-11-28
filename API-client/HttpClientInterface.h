#pragma once
#include <iostream>
#include <unordered_map>
using std::string;
using std::unordered_map;

typedef std::unordered_map<std::string, std::string> Header;

struct HttpResponse
{
    HttpResponse() : code(0) {}
    int code;
    Header responseHeaders;
    string strBody;
};

struct ByteBuffer;

#define _VCB virtual const bool
#define URL_AND_HEADERS const std::string &url, const Header &header
#define RESPONSE HttpResponse &response
#define VIRTUAL_REST_NO_WRITE_REQUEST(name) _VCB name(URL_AND_HEADERS, RESPONSE) const = 0;
#define VIRTUAL_REST_WRITE_REQUEST(name, data) _VCB name(URL_AND_HEADERS, data, RESPONSE) const = 0;

class HttpClientInterface
{

public:
    VIRTUAL_REST_NO_WRITE_REQUEST(Head);
    VIRTUAL_REST_NO_WRITE_REQUEST(Get);
    VIRTUAL_REST_NO_WRITE_REQUEST(Del);
    VIRTUAL_REST_WRITE_REQUEST(Post, const std::string &data);
    VIRTUAL_REST_WRITE_REQUEST(Put, const std::string &data);
    VIRTUAL_REST_WRITE_REQUEST(Put, const ByteBuffer &data);
    virtual ~HttpClientInterface(){};
};
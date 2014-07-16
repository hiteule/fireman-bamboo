#ifndef H_TEST
#define H_TEST

CURL *curl_handle;
CURLcode res;
XML_Parser parser;

struct ParserStruct {
    int ok;
    int parsed;
    int testState;
};
struct ParserStruct state;

static void startElement(void *data, const XML_Char *name, const XML_Char **atts);

static size_t parseStreamCallback(void *contents, size_t length, size_t nmemb, void *userp);

void setupParser();

void setupCurl();

void clean();

int getTestState();

#endif
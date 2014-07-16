#include <expat.h>
#include <curl/curl.h>
#include <string.h>
#include "test.h"

static void startElement(void *data, const XML_Char *name, const XML_Char **atts)
{
    struct ParserStruct *parserState = (struct ParserStruct *) data;

    if (!strcmp(name, "result") && parserState->parsed == 0) {
        parserState->parsed = 1;
        char key[42];
        char state[42];
        int i;
        for (i = 0; atts[i]; i += 2) {
            if (!strcmp(atts[i], "key")) {
                strcpy(key, atts[i + 1]);
            }
            if (!strcmp(atts[i], "state")) {
                strcpy(state, atts[i + 1]);
            }
        }

        if (!strcmp(state, "Failed")) {
            parserState->testState = 0;
        }

        if (getConfigDebug() == 1) {
            printf("%s => %s\n\n", key, state);
        }
    }
}
 
static size_t parseStreamCallback(void *contents, size_t length, size_t nmemb, void *userp)
{
    XML_Parser parser = (XML_Parser) userp;
    size_t real_size = length * nmemb;
    struct ParserStruct *state = (struct ParserStruct *) XML_GetUserData(parser);

    if (state->ok && XML_Parse(parser, contents, real_size, 0) == 0) {
        int error_code = XML_GetErrorCode(parser);
        fprintf(stderr, "Parsing failed. Buffer length : %lu . Error code : %d (%s).\n", real_size, error_code, XML_ErrorString(error_code));
        state->ok = 0;
    }

    return real_size;
}

void setupParser()
{
    memset(&state, 0, sizeof(struct ParserStruct));
    state.ok = 1;
    state.parsed = 0;
    state.testState = 1;

    parser = XML_ParserCreateNS(NULL, '\0');
    XML_SetUserData(parser, &state);
    XML_SetElementHandler(parser, startElement, NULL);
}

void setupCurl()
{
    SSL_library_init();
    curl_global_init(CURL_GLOBAL_ALL ^ CURL_GLOBAL_SSL);
    curl_handle = curl_easy_init();

    struct curl_slist *chunk = NULL;

    char httpAuthentStr[70];
    sprintf(httpAuthentStr, "Authorization: Basic %s", getConfigCredential());

    chunk = curl_slist_append(chunk, httpAuthentStr);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl_handle, CURLOPT_URL, getConfigTestUrl());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, parseStreamCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)parser);
}

void clean()
{
    XML_ParserFree(parser);
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}

int getTestState()
{
    setupParser();
    setupCurl();

    res = curl_easy_perform(curl_handle);
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        exit(-1);
    }

    if (XML_Parse(parser, NULL, 0, 1) == 0) {
        int error_code = XML_GetErrorCode(parser);
        fprintf(stderr, "Finalizing parsing failed with error code %d (%s).\n", error_code, XML_ErrorString(error_code));
        exit(-1);
    }

    clean();

    return state.testState;
}
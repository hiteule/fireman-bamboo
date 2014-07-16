#ifndef H_CONFIG
#define H_CONFIG

typedef struct
{
    int debug;
    int gpioWarningLight;
    const char* credential;
    const char* testUrl;
} configuration;

configuration config;

static int configHandler(void* user, const char* section, const char* name, const char* value);

void setupConfig(char* configFilePath);

int getConfigDebug();

int getConfigGpioWarningLight();

const char* getConfigCredential();

const char* getConfigTestUrl();


#endif
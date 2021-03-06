#ifndef H_CONFIG
#define H_CONFIG

typedef struct
{
    int debug;
    int gpioWarningLight;
    int gpioBuzzer;
    const char* credential;
    const char* testUrl;
    const char* bashSuccess;
    const char* bashFail;
} configuration;

configuration config;

static int configHandler(void* user, const char* section, const char* name, const char* value);

void setupConfig(char* configFilePath);

int getConfigDebug();

int getConfigGpioWarningLight();

int getConfigGpioBuzzer();

const char* getConfigCredential();

const char* getConfigTestUrl();

const char* getConfigBashSucess();

const char* getConfigBashFail();

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "inih/ini.h"

static int configHandler(void* user, const char* section, const char* name, const char* value)
{
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("config", "debug")) {
        pconfig->debug = atoi(value);
    } else if (MATCH("config", "gpio_warning_light")) {
        pconfig->gpioWarningLight = atoi(value);
    } else if (MATCH("config", "gpio_buzzer")) {
        pconfig->gpioBuzzer = atoi(value);
    } else if (MATCH("config", "credential")) {
        pconfig->credential = strdup(value);
    } else if (MATCH("config", "test_url")) {
        pconfig->testUrl = strdup(value);
    } else {
        return 0;
    }
    return 1;
}

void setupConfig(char* configFilePath)
{
    if (ini_parse(configFilePath, configHandler, &config) < 0) {
        fprintf(stderr, "Can't load '%s' config file.\n", configFilePath);
        exit(-1);
    }
}

int getConfigDebug()
{
    return config.debug;
}

int getConfigGpioWarningLight()
{
    return config.gpioWarningLight;
}

int getConfigGpioBuzzer()
{
    return config.gpioBuzzer;
}

const char* getConfigCredential()
{
    return config.credential;
}

const char* getConfigTestUrl()
{
    return config.testUrl;
}
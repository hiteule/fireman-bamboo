#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include "test.h"
#include "config.h"

int main(int argc, char *argv[], char **envp)
{
    if (argc != 2) {
        fprintf(stderr, "Bad argument. Usage : %s [config file path]\n", argv[0]);
        exit(-1);
    }

    setupConfig(argv[1]);
    setupGpio();

    int warningLightState = getWarningLightState();
    int testState         = getTestState();

    if (testState == 0) {
        if (getConfigDebug() == 1) {
            printf("TEST NOK\n");
        }

        if (!warningLightState) {
            char *bashFailFilePath = getConfigBashFail();

            if (0 != strcmp(bashFailFilePath, "false")) {
                runBashScript(bashFailFilePath);
            }

            setWarningLightState(1);
            int i;
            for (i = 0; i<2; i++) {
                morse();
                sleep(1);
            }
        }
    } else {
        if (getConfigDebug() == 1) {
            printf("TEST OK\n");
        }

        if (warningLightState) {
            setWarningLightState(0);
            victory();

            char *bashSuccessFilePath = getConfigBashSuccess();

            if (0 != strcmp(bashSuccessFilePath, "false")) {
                runBashScript(bashSuccessFilePath);
            }
        }
    }

    return 0;
}

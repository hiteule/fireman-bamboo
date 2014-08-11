#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "gpio.h"

void setupGpio()
{
    if ((gpio_mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0) {
        fprintf(stderr, "open /dev/mem failed.\n");
        exit(-1);
    }

    gpio_map = mmap(
        0,
        GPIO_BLOCK_SIZE,
        PROT_READ|PROT_WRITE,
        MAP_SHARED,
        gpio_mem_fd,
        GPIO_BASE
    );

    close(gpio_mem_fd);

    if (gpio_map == MAP_FAILED) {
        fprintf(stderr, "mmap error %d\n", (int)gpio_map);
        exit(-1);
    }

    gpio = (volatile unsigned *)gpio_map;

    gpioWarningLight = getConfigGpioWarningLight();
    gpioBuzzer       = getConfigGpioBuzzer();

    OUT_GPIO(gpioWarningLight);
    OUT_GPIO(gpioBuzzer);
}

int getWarningLightState()
{
    return ((GPIO_LEV & (1 << gpioWarningLight)) != 0);
}

void setWarningLightState(int state)
{
    if (state == 1) {
        if (getConfigDebug() == 1) {
            printf("Set warning light output = 1 (warning light on)\n");
        }
        GPIO_SET = 1<<gpioWarningLight;
    } else {
        if (getConfigDebug() == 1) {
            printf("Set warning light output = 0 (warning light off)\n");
        }
        GPIO_CLR = 1<<gpioWarningLight;
    }
}

void buzz(int duration)
{
    int delay = 200;
    int cycles = (delay * duration);

    int i;
    for (i=0; i<cycles; i++) {
        GPIO_SET = 1<<gpioBuzzer;
        usleep(delay);
        GPIO_CLR = 1<<gpioBuzzer;
        usleep(delay);
    }
}

void morse()
{
    buzz(1); // .
    usleep(150000);
    buzz(1); // .
    usleep(150000);
    buzz(1); // .
    usleep(150000);
    buzz(2); // -
    usleep(150000);
    buzz(2); // -
    usleep(150000);
    buzz(2); // -
    usleep(150000);
    buzz(1); // .
    usleep(150000);
    buzz(1); // .
    usleep(150000);
    buzz(1); // .
}

void victory()
{
    buzz(1);
    usleep(250000);
    buzz(1);
    usleep(250000);
    buzz(1);
    usleep(150000);
    buzz(1);
    usleep(150000);
    buzz(1);
    usleep(250000);
    buzz(1);
    usleep(150000);
    buzz(1);
    usleep(150000);
    buzz(1);
    usleep(150000);
    buzz(1);
    usleep(250000);
    buzz(1);
    usleep(150000);
    buzz(1);
}
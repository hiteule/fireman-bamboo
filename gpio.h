#ifndef H_GPIO
#define H_GPIO

#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000)
#define GPIO_BLOCK_SIZE (4*1024)

#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)
#define GPIO_CLR *(gpio+10)
#define GPIO_LEV *(gpio+13)

int gpioWarningLight;
int gpioBuzzer;
int gpio_mem_fd;
void *gpio_map;
volatile unsigned *gpio;

void setupGpio();

int getWarningLightState();

void setWarningLightState(int state);

void buzz(int duration);

void morse();

void victory();

#endif
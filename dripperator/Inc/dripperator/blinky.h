#include "gpio.h"

#define BLINKY_TICK_RATE_HZ (3)
#define BLINKY_GPIO_PORT (LED_GREEN_GPIO_Port)
#define BLINKY_GPIO_PIN (LED_GREEN_Pin)

void update_blinky();
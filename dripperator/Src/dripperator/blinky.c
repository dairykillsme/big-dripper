#include "dripperator/blinky.h"

static uint32_t last_tick = 0;
static uint8_t output_state = GPIO_PIN_RESET;

int update_blinky() {
    uint32_t tick = HAL_GetTick();
    if (tick - last_tick > (1000 / BLINKY_TICK_RATE_HZ)) {
        last_tick = tick;
        output_state = ~output_state;
        HAL_GPIO_WritePin(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN, output_state);
    }
}
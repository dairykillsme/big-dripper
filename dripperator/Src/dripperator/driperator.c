#include "dripperator/dripperator.h"

#define NUM_SOLENOIDS (8)
#define SWITCH_DUTY_CYCLE (4096)
#define SWITCH_TIME_MS (1000)
#define HOLD_DUTY_CYCLE (2048)


solenoid_config_t solenoid_configs [NUM_SOLENOIDS] = {
    {
        .tim_handle = &htim1,
        .tim_channel = TIM_CHANNEL_1,
        .switch_duty_cycle = SWITCH_DUTY_CYCLE,
        .switch_time_ms = SWITCH_TIME_MS,
        .hold_duty_cycle = HOLD_DUTY_CYCLE,
    },
    {
        .tim_handle = &htim1,
        .tim_channel = TIM_CHANNEL_2,
        .switch_duty_cycle = SWITCH_DUTY_CYCLE,
        .switch_time_ms = SWITCH_TIME_MS,
        .hold_duty_cycle = HOLD_DUTY_CYCLE,
    },
    {
        .tim_handle = &htim1,
        .tim_channel = TIM_CHANNEL_3,
        .switch_duty_cycle = SWITCH_DUTY_CYCLE,
        .switch_time_ms = SWITCH_TIME_MS,
        .hold_duty_cycle = HOLD_DUTY_CYCLE,
    },
    {
        .tim_handle = &htim1,
        .tim_channel = TIM_CHANNEL_4,
        .switch_duty_cycle = SWITCH_DUTY_CYCLE,
        .switch_time_ms = SWITCH_TIME_MS,
        .hold_duty_cycle = HOLD_DUTY_CYCLE,
    },
    {
        .tim_handle = &htim3,
        .tim_channel = TIM_CHANNEL_1,
        .switch_duty_cycle = SWITCH_DUTY_CYCLE,
        .switch_time_ms = SWITCH_TIME_MS,
        .hold_duty_cycle = HOLD_DUTY_CYCLE,
    },
    {
        .tim_handle = &htim3,
        .tim_channel = TIM_CHANNEL_2,
        .switch_duty_cycle = SWITCH_DUTY_CYCLE,
        .switch_time_ms = SWITCH_TIME_MS,
        .hold_duty_cycle = HOLD_DUTY_CYCLE,
    },
    {
        .tim_handle = &htim3,
        .tim_channel = TIM_CHANNEL_3,
        .switch_duty_cycle = SWITCH_DUTY_CYCLE,
        .switch_time_ms = SWITCH_TIME_MS,
        .hold_duty_cycle = HOLD_DUTY_CYCLE,
    },
    {
        .tim_handle = &htim3,
        .tim_channel = TIM_CHANNEL_4,
        .switch_duty_cycle = SWITCH_DUTY_CYCLE,
        .switch_time_ms = SWITCH_TIME_MS,
        .hold_duty_cycle = HOLD_DUTY_CYCLE,
    },
};

solenoid_handle_t solenoids [NUM_SOLENOIDS];

void driperator_init() {
    for (size_t i = 0; i < NUM_SOLENOIDS; i++) {
        solenoids[i] = solenoid_init(solenoid_configs[i]);
        solenoid_write_output(&solenoids[i], GPIO_PIN_SET);
    }
}

void driperator_update() {
    update_blinky();
    for (size_t i = 0; i < NUM_SOLENOIDS; i++) {
        solenoid_update(&solenoids[i]);
    }
}
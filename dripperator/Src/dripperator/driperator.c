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

communcator_config_t communicator_config = {
    .uart_handle = &huart1,
};

void driperator_init() {
    for (size_t i = 0; i < NUM_SOLENOIDS; i++) {
        solenoids[i] = solenoid_init(solenoid_configs[i]);
        solenoid_write_output(&solenoids[i], GPIO_PIN_SET);
    }
    communicator_1 = communicator_init(communicator_config);
}

void driperator_update() {
    update_blinky();
    for (size_t i = 0; i < NUM_SOLENOIDS; i++) {
        solenoid_update(&solenoids[i]);
    }
    // If we have recieved a complete frame, deploy da solenoids
    if (communicator_update(&communicator_1)) {
        uint8_t solenoid_outputs = communicator_get_last_solenoid_command(&communicator_1);
        for (size_t i = 0; i < NUM_SOLENOIDS; i++) {
            uint8_t output_value = ((solenoid_outputs & (1 << i)) != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
            solenoid_write_output(&solenoids[i], output_value);
        }
    }
}
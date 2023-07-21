#include "dripperator/solenoid.h"

solenoid_handle_t solenoid_init(solenoid_config_t config) {
    solenoid_handle_t solenoid = {
        .config = config,
        .last_switch_at_ms = 0,
        .current_state = GPIO_PIN_RESET,
    };
    HAL_TIM_PWM_Start(config.tim_handle, config.tim_channel);
    HAL_TIM_Base_Start(config.tim_handle);
    return solenoid;
}

void solenoid_write_output(solenoid_handle_t *solenoid, uint8_t state) {
    if (state == solenoid->current_state) {return;}
    solenoid->current_state = state;
    if (state == GPIO_PIN_RESET) {
        __HAL_TIM_SetCompare(solenoid->config.tim_handle, solenoid->config.tim_channel, 0);
    } else if (state == GPIO_PIN_SET) {
        __HAL_TIM_SetCompare(solenoid->config.tim_handle, solenoid->config.tim_channel, solenoid->config.switch_duty_cycle);
        solenoid->last_switch_at_ms = HAL_GetTick();
    }
}

void solenoid_update(solenoid_handle_t *solenoid) {
    if (solenoid->current_state == GPIO_PIN_RESET) {return;}
    // Switch to hold duty cycle after switch time has passed
    if ((HAL_GetTick() - solenoid->last_switch_at_ms) > solenoid->config.switch_time_ms) {
        __HAL_TIM_SetCompare(solenoid->config.tim_handle, solenoid->config.tim_channel, solenoid->config.hold_duty_cycle);
    }
}
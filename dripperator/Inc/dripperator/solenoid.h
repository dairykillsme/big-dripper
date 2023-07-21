#include "tim.h"

typedef struct solenoid_config {
    /* Handle for timer used to PWM this output*/
    TIM_HandleTypeDef *tim_handle;
    /* Channel of timer used to PWM this output*/
    uint32_t tim_channel;
    /* Output duty cycle to switch at*/
    uint16_t switch_duty_cycle;
    /* How long to apply switching duty cycle before falling to holding duty cycle*/
    uint16_t switch_time_ms;
    /* Output duty cycle to hold solenoid open at*/
    uint16_t hold_duty_cycle;
} solenoid_config_t;

typedef struct solenoid_handle {
    /* Solenoid config */
    solenoid_config_t config;
    /* When was last hold cycle triggered*/
    uint32_t last_switch_at_ms;
    /* current state*/
    uint8_t current_state;
} solenoid_handle_t;

/**
 * @brief Initialize the solenoid 
 * 
 * @param config Handle for the solenoid
 * @return A new solenoid handle
 */
solenoid_handle_t solenoid_init(solenoid_config_t config);

/**
 * @brief Set the solenoid output
 * 
 * @param solenoid
 */
void solenoid_write_output(solenoid_handle_t *solenoid, uint8_t state);

/**
 * @brief Update the solenoid internal state machine
 * 
 * @param solenoid 
 */
void solenoid_update(solenoid_handle_t *solenoid);
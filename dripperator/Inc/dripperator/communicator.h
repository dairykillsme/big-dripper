#include "usart.h"
#include "gpio.h"

typedef enum communicator_state {
    COMMUNICATOR_STATE_INIT = 0,
    COMMUNICATOR_STATE_WAIT_FOR_HEADER,
    COMMUNICATOR_RECEPTION_COMPLETE,
} communicator_state_t;

typedef struct communcator_config {
    UART_HandleTypeDef *uart_handle;
} communcator_config_t;

typedef struct communicator_handle {
    communcator_config_t config;
    uint8_t address;
    uint8_t solenoid_outputs;
    communicator_state_t state;
    uint8_t rx_bytes [32];
} communicator_handle_t;

communicator_handle_t communicator_1;

communicator_handle_t communicator_init(communcator_config_t config);

uint8_t communicator_update(communicator_handle_t *communicator);

uint8_t communicator_get_last_solenoid_command(communicator_handle_t *communicator);
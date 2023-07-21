#include "dripperator/communicator.h"
#include <string.h>

typedef struct communicator_header {
    uint8_t sync : 1;
    uint8_t reserved: 3;
    uint8_t address: 4;
} communicator_header_t;

communicator_handle_t communicator_init(communcator_config_t config) {
    // Read address from dip switch pins
    uint8_t address = (HAL_GPIO_ReadPin(ADDR_3_GPIO_Port, ADDR_3_Pin) << 3) +
                      (HAL_GPIO_ReadPin(ADDR_2_GPIO_Port, ADDR_2_Pin) << 2) +
                      (HAL_GPIO_ReadPin(ADDR_1_GPIO_Port, ADDR_1_Pin) << 1) +
                      (HAL_GPIO_ReadPin(ADDR_0_GPIO_Port, ADDR_0_Pin) << 0);

    communicator_handle_t communicator = {
        .config = config,
        .address = address,
        .solenoid_outputs = 0,
        .state = COMMUNICATOR_STATE_INIT,
        .rx_bytes = {0},
    };

    return communicator;
}

uint8_t communicator_update(communicator_handle_t *communicator) {
    if (communicator->state == COMMUNICATOR_STATE_INIT) {
        HAL_UART_Receive_IT(communicator->config.uart_handle, communicator->rx_bytes, 2);
        communicator->state = COMMUNICATOR_STATE_WAIT_FOR_HEADER;
    }
    if (communicator->state == COMMUNICATOR_RECEPTION_COMPLETE) {
        communicator_header_t recv_header;
        memcpy(&recv_header, communicator->rx_bytes, 1);
        // If sync bit, return true
        if (recv_header.sync) {
            return 1;
        }
        // If address match, preload solenoid outputs value
        else if (recv_header.address == communicator->address) {
            communicator->solenoid_outputs = communicator->rx_bytes[1];
        }
        HAL_UART_Receive_IT(communicator->config.uart_handle, communicator->rx_bytes, 2);
        communicator->state = COMMUNICATOR_STATE_WAIT_FOR_HEADER;
    }
    return 0;
}

uint8_t communicator_get_last_solenoid_command(communicator_handle_t *communicator) {
    return communicator->solenoid_outputs;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    communicator_1.state = COMMUNICATOR_RECEPTION_COMPLETE;
}


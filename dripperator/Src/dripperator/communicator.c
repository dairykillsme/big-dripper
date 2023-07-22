#include "dripperator/communicator.h"
#include <string.h>

typedef struct communicator_header {
    uint8_t address: 4;
    uint8_t reserved: 3;
    uint8_t sync : 1;
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
        memset(communicator->rx_bytes, 0, 32);
        HAL_UART_Receive_IT(communicator->config.uart_handle, communicator->rx_bytes, 32);
        communicator->state = COMMUNICATOR_STATE_WAIT_FOR_HEADER;
    }
    if (communicator->state == COMMUNICATOR_STATE_WAIT_FOR_HEADER) {
        for (size_t i = 0; i < 32; i += 2) {
            communicator_header_t *recv_header = (communicator_header_t *) (communicator->rx_bytes + i);
            if (recv_header->sync) {
                communicator_1.state = COMMUNICATOR_RECEPTION_COMPLETE;
            }
        }
    }
    if (communicator->state == COMMUNICATOR_RECEPTION_COMPLETE) {
        HAL_UART_Abort_IT(communicator->config.uart_handle);
        // Check every other byte for message headers
        for (size_t i = 0; i < 32; i += 2) {
            communicator_header_t *recv_header = (communicator_header_t *) (communicator->rx_bytes + i);
            if (!recv_header->sync && recv_header->address == communicator->address) {
                communicator->solenoid_outputs = communicator->rx_bytes[i + 1];
            }
            if (recv_header->sync) {
                communicator->state = COMMUNICATOR_STATE_INIT;
                return 1;
            }
        }
        // We shouldn't get here, but if we do, restart everything
        communicator->state = COMMUNICATOR_STATE_INIT;
        return 1;
    }
    return 0;
}

uint8_t communicator_get_last_solenoid_command(communicator_handle_t *communicator) {
    return communicator->solenoid_outputs;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    communicator_1.state = COMMUNICATOR_RECEPTION_COMPLETE;
}


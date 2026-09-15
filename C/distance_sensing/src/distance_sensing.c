/*
    author: Henry Centeno
    description:
        This programs takes in the data transmitted by the mcu to print out the distance it detected the object at.
*/

#include "lpuart_server/lpuart_server.h"

void distance_handler(const uint8_t *data, uint8_t length){
    float distance;
    memcpy(&distance, data, length);
    printf("Detected object at %f inches.\n", distance);
}

int main(void){
    int port = init_lpuart_server();
    if(port < 0){
        perror("open");
        return 1;
    }

    run_lpuart_server(port, distance_handler);
    close_lpuart_server(port);

    return 0;
}
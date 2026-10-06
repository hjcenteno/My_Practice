/*
    author: Henry Centeno
    description: read the data from the mpu6050 to verify who it is
*/

#include "lpuart_server/lpuart_server.h"

void handler(const uint8_t *data, uint8_t length){
    //I am expecting just 1 byte
    uint8_t actual;
    memcpy(&actual, data, length);
    printf("%02x\n", actual);
}

int main(void){
    int port = init_lpuart_server();
    if(port < 0){
        perror("open");
        return 1;
    }

    run_lpuart_server(port, handler);
    close_lpuart_server(port);

    return 0;
}
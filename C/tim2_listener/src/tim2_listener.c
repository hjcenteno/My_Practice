/*
    author: Henry Centeno
    description:
        To test the mcu_time implementation from Embedded Projects
*/

#include "lpuart_server/lpuart_server.h"
#include "stdbool.h"

void tim_handler(const uint8_t *data, uint8_t length){
    //check the value of the bool
    char msg[length];
    memcpy(msg, data, length);
    printf("%s\n", msg);
}

int main(void){
    int port = init_lpuart_server();
    if(port < 0){
        perror("open");
        return 1;
    }

    run_lpuart_server(port, tim_handler);
    close_lpuart_server(port);

    return 0;
}

/*
    author: Henry Centeno
    description: 
        This program will listen for whenever the user presses a button on the mcu to state that the button has been pressed and released
*/

#include "lpuart_server/lpuart_server.h"

//test function
// void print_handler(const uint8_t *data, uint8_t length){
//     printf("read in %u bytes.\n", length);

//     for(uint8_t i = 0; i < length; i++){
//         if(i > MAX_PAYLOAD_SIZE){
//             printf("\n");
//         }
//         printf("%02x ", data[i]);
//     }

//     printf("\n");
// }

void button_state_handler(const uint8_t *data, uint8_t length){
    uint8_t state = (uint8_t)*data;
    if(state == 1){
        printf("button has been pressed\n");
    }else{
        printf("button is not being pressed\n");
    }
}

int main(void){
    int server = init_lpuart_server();
    if(server < 0){
        perror("open");
        return -1;
    }

    run_lpuart_server(server, button_state_handler); //runs continously
    close_lpuart_server(server);

    return 0;
}
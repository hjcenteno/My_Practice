/*
    author: Henry Centeno
    description:
        This will read in the state of the button as a uint32_t to match the register of the BSSR for the GPIO peripheral. 
        To read in the state of any button, I will first see if the bit position & the button state is true.
*/

#include "lpuart_server/lpuart_server.h"

#define button1 (0x1ul << (5u)) //copy of the GPIO_IDR_IDR_5 definition
#define button2 (0x1ul << (4u))
#define button3 (0x1ul << (10u)) 

/*
    recall that we are sending a raw unsigned 32 bit integer where bits[0:15] are used in the IDR for each GPIO
    Since, button 3 is connected to gpioa, but buttons 1 and 2 are connected to gpioc, it is important to remember that
    the current information being transmitted would only support up to 16 unique numbers agnostic to its gpio peripheral.
*/
void which_button_handler(const uint8_t *data, uint8_t length){
    /*
        checks the bit to see if its 1 to see which button was pressed.
        the overall algorithm is similar to the led_selector.c in my embedded_projects repo
    */

    uint32_t buttonState = 0;
    memcpy(&buttonState, data, length); //copy the data into the state

    if(buttonState & button1){ //say that button 1 was turned on
        printf("button 1 was pressed\n");
    }

    if(buttonState & button2){ //say that button 1 was turned on
        printf("button 2 was pressed\n");
    }

    if(buttonState & button3){ //say that button 1 was turned on
        printf("button 3 was pressed\n");
    }

    if(buttonState == 0){
        printf("No button was pressed\n");
    }

}

int main(void){
    int port = init_lpuart_server();
    if(port < 0){
        perror("open");
        return -1;
    }

    run_lpuart_server(port, which_button_handler);
    close_lpuart_server(port);

    return 0;
}

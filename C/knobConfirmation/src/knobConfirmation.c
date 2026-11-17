/*
    author: Henry Centeno
    description:
        This is to determine the meaning of s1, s2, and key on my encoder
*/

#include "lpuart_server/lpuart_server.h"
#define GPIO_IDR_ID3_Pos               (3U)
#define GPIO_IDR_ID3_Msk               (0x1UL << GPIO_IDR_ID3_Pos)             /*!< 0x00000008 */
#define GPIO_IDR_ID3                   GPIO_IDR_ID3_Msk
#define GPIO_IDR_ID4_Pos               (4U)
#define GPIO_IDR_ID4_Msk               (0x1UL << GPIO_IDR_ID4_Pos)             /*!< 0x00000010 */
#define GPIO_IDR_ID4                   GPIO_IDR_ID4_Msk
#define GPIO_IDR_ID5_Pos               (5U)
#define GPIO_IDR_ID5_Msk               (0x1UL << GPIO_IDR_ID5_Pos)             /*!< 0x00000020 */
#define GPIO_IDR_ID5                   GPIO_IDR_ID5_Msk


typedef struct KnobTransmission{
    float rotationAmt;
    float delta;
} KnobTransmission;

void knob_handler(const uint8_t *data, uint8_t length){
    if(length != sizeof(KnobTransmission)){
        return; //data is not the correct length
    }

    //check what bit is read in
    KnobTransmission kt;
    memcpy(&kt, data, sizeof(kt));

    //first flip idr since the pin would be 0 on logic low, then see if it's 1
    printf("delta: %.02f\n", kt.delta);
    printf("rotationAmt: %.02f\n", kt.rotationAmt);
}

int main(void){
    int port = init_lpuart_server();
    if(port < 0){
        perror("open");
        return 1;
    }

    run_lpuart_server(port, knob_handler);
    close_lpuart_server(port);

    return 0;
}
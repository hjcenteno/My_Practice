/*
    author: Henry Centeno
    description:
        this project will handle listening to what the mcu transmits of what it read of the mpu6050
*/

#include "lpuart_server/lpuart_server.h"
#include <string.h>

typedef struct mpu6050_t{
    uint16_t accX;
    uint16_t accY;
    uint16_t accZ;
    uint16_t gyroX;
    uint16_t gyroY;
    uint16_t gyroZ;
    uint16_t temp;
}mpu6050_t;


void mpu6050_handler(const uint8_t *data, uint8_t length){
    //first transmission is the whoami
    //second transmission is the mpu6050 struct
    char *errmsg = "[error] did not read expected whoAmI value.";

    if(length == sizeof(mpu6050_t)){ //we know we've received the struct
        printf("mpu6050_t struct (raw bytes):");
        for(uint8_t i = 0; i < length; i++){
            if(i % 2 == 0){
                printf("\n");
            }

            printf("    0x%02x  ", data[i]);
        }
    }
    else if(memcmp((const char *)data, errmsg, 44) == 0){ //check if they're equal
        printf("[error] did not read expected whoAmI value.\n");
    }

    else{
        uint8_t iAm;
        memcpy(&iAm, data, length);
        printf("I am 0x%02x\n", iAm); //print whoAmI
    }
    printf("\n");
}

int main(void){
    int server = init_lpuart_server();
    if(server < 0){
        perror("open");
        return 1;
    }

    run_lpuart_server(server, mpu6050_handler);
    close_lpuart_server(server);

    return 0;
}
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

typedef struct accelf_t{
    float x;
    float y;
    float z;
}accelf_t;

void mpu6050_handler(const uint8_t *data, uint8_t length){
    //first transmission is the whoami
    //second transmission is the mpu6050 struct

    if(length == sizeof(mpu6050_t)){ //we know we've received the struct
        printf("mpu6050_t struct (raw bytes):");
        for(uint8_t i = 0; i < length; i++){
            if(i % 2 == 0){
                printf("\n");
            }

            printf("    {%d}0x%02x  ", i,  data[i]);
        }
    }
    else if(length == sizeof(accelf_t)){
        accelf_t accRx;
        memcpy(&accRx, data, sizeof(accRx));
        printf("x: %.04f y: %.04f z: %.04f\n", accRx.x, accRx.y, accRx.z);
        // printf("X: %.04f\n", testingAxis);
        printf("\n");
    }
    else{
        printf("I am %#x\n", data[0]); //print whoAmI
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
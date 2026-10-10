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

typedef struct cal_mpu6050_t{
    //holds the data to do math from the raw data
    float accX;
    float accY;
    float accZ;
    float temp;
    float gyroRoll;
    float gyroPitch;
    float gyroYaw;
}cal_mpu6050_t;

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
    else if(length == sizeof(cal_mpu6050_t)){
        cal_mpu6050_t cal_data;
        //need to study of a more better/secure way of handling the raw bytes
        memcpy(&cal_data, data, sizeof(cal_data));
        
        /* output format:
            accelerometer data: 
                x: %f, y: %f, z: %f

            gyrometer data:
                roll: %f, pitch: %f, yaw: %f
        */
        printf( //print accelerometer data
            "accelerometer data:\n    x: %.04f, y: %.04f, z: %.04f\n\n",
            cal_data.accX, cal_data.accY, cal_data.accZ
        );

        printf( //print gyrometer data
            "gyrometer data:\n    x: %.04f, y: %.04f, z: %.04f\n",
            cal_data.gyroRoll, cal_data.gyroPitch, cal_data.gyroYaw
        );
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
/*
    author: Henry Centeno
    description:
        The purpose of the program is to listen to the pitch and role angles reported by the mpu6050
*/

#include "lpuart_server/lpuart_server.h"

typedef struct orientation{
    float roll;
    float pitch;
}orientation;

typedef struct raw_mpu6050_t{
    //matches how where each value is in stored registers
    int16_t accX;
    int16_t accY;
    int16_t accZ;
    int16_t temp;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
}raw_mpu6050_t;

typedef struct cal_mpu6050_t{
    //holds the data to do math from the raw data
    float accX;
    float accY;
    float accZ;
    float temp;
    float gyroX;
    float gyroY;
    float gyroZ;
}cal_mpu6050_t;

/*  outline of the transmission:
first transmission is the whoAmI register value
second transmission will be the cal and raw mpu6050 structs
third transmission will the accelerometer and gyrometer data
fourth transmission will be the pitch and roll angles
*/
void angle_handler(const uint8_t *data, uint8_t length){
    switch (length){
        case sizeof(orientation):
            orientation angles;
            memcpy(&angles, data, sizeof(angles));
            printf("roll: %04f radians, pitch: %04f radians\n", angles.roll, angles.pitch);
            break;
        case sizeof(raw_mpu6050_t):
            //just print the raw bytes
            printf("mpu6050_t struct (raw bytes):");
            for(uint8_t i = 0; i < length; i++){
                if(i % 2 == 0){
                    printf("\n");
                }
                
                printf("    {%d}0x%02x  ", i,  data[i]);
            }
            break;
        case sizeof(cal_mpu6050_t):
            //print the accelerometer and gyrometer data
            cal_mpu6050_t cal_data;
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
                cal_data.gyroX, cal_data.gyroY, cal_data.gyroZ
            );
            break;
        case sizeof(uint8_t):
            printf("I am %#x\n", data[0]); //print whoAmI
        default:
            break;
    }
    printf("\n");
}

int main(void){
    int port = init_lpuart_server();
    if(port < 0){
        perror("open");
        return 1;
    }

    run_lpuart_server(port, angle_handler);
    close_lpuart_server(port);

    return 0;
}

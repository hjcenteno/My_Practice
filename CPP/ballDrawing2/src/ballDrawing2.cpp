/*
    author: Henry Centeno
    description:
        This will expand upon ballDrawing but input will come from the mpu6050.

    Version 0.0:
        First thing is to have the listener happen in the background of the window.
*/

#include <inttypes.h>
#include <stdio.h>
#include <thread>
#include <atomic>
#include "headers/common.hpp"
#include "headers/orientationHandling.hpp"
#include "headers/ballPlayer.hpp"
#include "C/lib/lpuart_server/lpuart_server.h"

#define WIDTH (1200u)
#define HEIGHT (800u)
#define SPEED (500.0f)

using std::thread;

std::atomic_bool sensorConnected = false;


void renderPlayer(BallPlayer &player){
    //handle drawing the player in the window
    BeginDrawing();
    ClearBackground(BLANK);
    DrawCircleV(player.position, player.radius, RED);
    EndDrawing();
}

/* The main function will need to be split into two threads:
 * one thread to run the raylib window
 * another thread to read the mpu6050
*/

/* outline of the transmission:
 * I will be ignoring the first three transmissions so I do have to build new firmware
 * fourth transmission will be the pitch and roll angles
*/
void uart_angleHandler(const uint8_t *data, uint8_t length){
    switch(length){
        case sizeof(orientation):
            orientation angles;
            memcpy(&angles, data, sizeof(angles));
            printf("roll: %04f radians, pitch: %04f radians\n", angles.roll, angles.pitch);
            break;
        default:
            break;
    }
}

int init_lpuart_listening(){
    int port = init_lpuart_server();
    if(port < 0){
        perror("open");
        return 1;
    }

    atomic_store(&sensorConnected, true); //signal the sensor is connected
    run_lpuart_server(port, uart_angleHandler);
    close_lpuart_server(port);
    
    return 0;
}

int main(void){
    InitWindow(WIDTH, HEIGHT, "Ball Drawing Test");
    float initRadius = 15.0; //initial radius of the ball
    BallPlayer player = BallPlayer(initRadius); //radius of 15
    Vector2 currAcceleration = {0.0, 0.0};

    thread mpu6050Listener(&init_lpuart_listening);

    while(!WindowShouldClose()){
        if(sensorConnected){
            captureSensorInputs(currAcceleration);
        }
        else{
            captureKeyInputs(currAcceleration);
        }
        processAccelerationInput(currAcceleration, player);
        renderPlayer(player);
    }
    stop_lpuart_thread(false); //false since the server is just reading
    mpu6050Listener.join();
    CloseWindow();
    return 0;
}
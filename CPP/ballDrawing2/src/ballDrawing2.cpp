/*
    author: Henry Centeno
    description:
        This will expand upon ballDrawing but input will come from the mpu6050.

    Version 0.0:
        First thing is to have the listener happen in the background of the window.
*/

#include "headers/common.hpp"
#include "headers/orientationHandling.hpp"
#include "headers/ballPlayer.hpp"
#include "C/lib/lpuart_server/lpuart_server.h"

#define WIDTH (1200u)
#define HEIGHT (800u)
#define SPEED (500.0f)

using std::thread;

/* at the moment, orientation is just pitch and roll so it's just 8 bytes at the moment.
 * If yaw is added, the size of orientation will be 12 making this file uncompilable until
 * the yaw is handled
 */
static_assert(sizeof(orientation) == 8);

std::atomic_bool sensorConnected = false;
std::atomic<orientation> atomic_angles{ 
    orientation{ //set atomic angles as an orientation of 0, 0
        .roll = 0.0f, 
        .pitch = 0.0f
    } 
}; //where the pitch and roll angles will be stored

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
    //the purpose of this function is to collect the data from the mpu6050
    switch(length){
        case sizeof(orientation):
            orientation dataBuffer;
            //copying the memory into the buffer is safer than casting data and reading the bytes directly as a orientation
            memcpy(&dataBuffer, data, sizeof(dataBuffer));
            atomic_angles.store(dataBuffer);
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
    atomic_store(&sensorConnected, false); //signal the sensor is not connected
    
    return 0;
}

int main(void){
    InitWindow(WIDTH, HEIGHT, "Ball Drawing Test");
    SetTargetFPS(60);

    float initRadius = 15.0; //initial radius of the ball
    BallPlayer player = BallPlayer(initRadius); //radius of 15
    Vector2 currAcceleration = {0.0, 0.0};

    thread mpu6050Listener(&init_lpuart_listening);

    while(!WindowShouldClose()){
        if(sensorConnected){
            captureSensorInputs(currAcceleration, atomic_angles);
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
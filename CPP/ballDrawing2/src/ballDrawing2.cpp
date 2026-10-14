/*
    author: Henry Centeno
    description:
        This will expand upon ballDrawing but input will come from the mpu6050.

    Version 0.0:
        First thing is to have the listener happen in the background of the window.
*/

#include <raylib.h>
#include <raymath.h>
#include <inttypes.h>
#include <stdio.h>
#include <thread>
#include "C/lib/lpuart_server/lpuart_server.h"

#define WIDTH (1200u)
#define HEIGHT (800u)
#define SPEED (500.0f)

using std::thread;

constexpr float ACCELERATION = 1.0f;

/* The position of the ball will be determined by the roll/pitch angles from the mpu6050
 * 
 */

class ball{
    private:
        //positions
            

    public:
        ball(float r); //establishes the initial x and y position of the ball on the canvas
        ~ball(); //free it

        void updatePosition(float dx, float dy);

        //public member variables
        Vector2 position;
        float radius;
};

ball::ball(float r){
    //draw the ball 
    // ball::x = WIDTH / 2; //initially at the center of the window
    // ball::y = HEIGHT / 2;
    ball::position = {WIDTH / 2, HEIGHT / 2};
    ball::radius = r;
}

ball::~ball(){
}

void ball::updatePosition(float dx, float dy){
    //get the new position of the where the ball would be
    float newX = position.x + dx;
    float newY = position.y + dy;

    //check if the position is in bounds
    bool xBound = (newX > radius) && (newX < ((float)WIDTH - radius));
    bool yBound = (newY > radius) && (newY < ((float)HEIGHT - radius));
    if(xBound && yBound){
        position.x = newX;
        position.y = newY;
    }
}

/* This will be split between 3 events: inputs, processing, then rendering.
*Inputs: the loop will handle obtaining the inputs then gives it to the ball class to do whatever it needs to.
This is to keep the ball class simple in that it should only worry about where the ball should be.

*Processing: This will handle keeping the ball in bounds of the window mainly. Perhaps acceleration too?
At the end of processing, it will give the processed inputs to the ball class

*Rendering: This will be the stuff between the begin and end drawing functions
*/

void captureAccelerationInput(Vector2 &directions){
    float dx = 0.0f;
    float dy = 0.0f;
    if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)){
        dy = -ACCELERATION;
    }

    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){
        dy = ACCELERATION;
    }

    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
        dx = -ACCELERATION;
    }

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
        dx = ACCELERATION;
    }
    
    directions.x = dx;
    directions.y = dy;
}

void processAccerelationInput(Vector2 direction, ball &player){
    //get the current position of the player
    const float step = GetFrameTime() * SPEED;
    direction = Vector2Normalize(direction); //makes the dy, dx between [0, 1]

    player.updatePosition(direction.x * step, direction.y * step);
}

void renderPlayer(ball &player){
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

int init_lpuart_listening(){
    int port = init_lpuart_server();
    if(port < 0){
        perror("open");
        return 1;
    }

    run_lpuart_server(port, angle_handler);
    close_lpuart_server(port);
    
    return 0;
}

int main(void){
    InitWindow(WIDTH, HEIGHT, "Ball Drawing Test");
    float initRadius = 15.0; //initial radius of the ball
    ball player = ball(initRadius); //radius of 15
    Vector2 currAcceleration = {0.0, 0.0};

    thread mpu6050Listener(&init_lpuart_listening);

    while(!WindowShouldClose()){
        captureAccelerationInput(currAcceleration);
        processAccerelationInput(currAcceleration, player);
        renderPlayer(player);
    }

    mpu6050Listener.join();
    CloseWindow();
    return 0;
}
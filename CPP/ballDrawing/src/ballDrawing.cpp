/*
    author: Henry Centeno
    description:
        This prorgam is to successfully draw a circle to the screen and respond to user input
*/

#include <raylib.h>
#include <inttypes.h>
#include <stdio.h>

#define WIDTH (1200u)
#define HEIGHT (800u)
#define ACCELERATION_RATE (5u)

typedef enum accelFactor{
    noAccel, //do nothing
    accelUp, //accelerate up
    accelDown, //accelerate down
    accelLeft, //accelerate left 
    accelRight, //accelerate right
}accelFactor;

class ball{
    private:
        //positions
            

    public:
        ball(float r); //establishes the initial x and y position of the ball on the canvas
        ~ball(); //free it

        void updatePosition(int dx, int dy);

        //public member variables
        uint16_t x;
        uint16_t y;
        float radius;
};

ball::ball(float r){
    //draw the ball 
    ball::x = WIDTH / 2; //initially at the center of the window
    ball::y = HEIGHT / 2;
    ball::radius = r;
}

ball::~ball(){
}

void ball::updatePosition(int dx, int dy){
    //get the new position of the where the ball would be
    uint16_t newX = this->x + dx; //integer overflow will make the position greater than the width/height
    uint16_t newY = this->y + dy;
    
    //check if the new position lies in the bounds of the window    
    if(
        ((newX > 0) && (newY > 0)) && //top left corner
        ((newX < WIDTH) && (newY < HEIGHT)) //bottom right corner
    ){
        fprintf(stdout, "dx: %d; dy: %d", dx, dy);
        this->x = newX;
        this->y = newY;
    }

}

/* This will be split between 3 events: inputs, processing, then rendering.
*Inputs: the loop will handle obtaining the inputs then gives it to the ball class to do whatever it needs to.
This is to keep the ball class simple in that it should only worry about where the ball should be.

*Processing: This will handle keeping the ball in bounds of the window mainly. Perhaps acceleration too?
At the end of processing, it will give the processed inputs to the ball class

*Rendering: This will be the stuff between the begin and end drawing functions
*/

accelFactor captureAccelerationInput(accelFactor current){
    //handle key inputs and update the corresponding axis
    int key = GetKeyPressed(); //get the key that was pressed
    if(key == 0){ //0 means nothing was pressed
        return current;
    }

    //valid inputs are w, a, s, d and the arrow key
    switch (key){
        //make the player go up
        case KEY_W:
        case KEY_UP:
            return accelUp;

        //make the player go down
        case KEY_S:
        case KEY_DOWN:
            return accelDown;

        //make the player go left
        case KEY_A:
        case KEY_LEFT:
            return accelLeft;
        
        //make the player go right
        case KEY_D:
        case KEY_RIGHT:
            return accelRight;

        default:
            return noAccel;
    }
}

void processAccerelationInput(accelFactor factor, ball &player){
    //get the current position of the player
    int newX = 0;
    int newY = 0;
    switch (factor){
        case accelUp:
            fprintf(stdout, "going up\n");
            newY -= ACCELERATION_RATE;
            break;
            
        case accelDown:
            fprintf(stdout, "going down\n");
            newY += ACCELERATION_RATE;
            break;

        case accelLeft:
            fprintf(stdout, "going left\n");
            newX -= ACCELERATION_RATE;
            break;

        case accelRight:
            fprintf(stdout, "going right\n");
            newX += ACCELERATION_RATE;
            break;
        
        default:
            fprintf(stdout, "staying still\n");
            newX = 0;
            newY = 0;
            break;
    }

    player.updatePosition(newX, newY);
}


void renderPlayer(ball &player){
    //handle drawing the player in the window
    BeginDrawing();
    ClearBackground(BLANK);
    DrawCircle(player.x, player.y, player.radius, RED);
    EndDrawing();
}

int main(void){
    InitWindow(WIDTH, HEIGHT, "Ball Drawing Test");
    float initRadius = 15.0; //initial radius of the ball
    ball player = ball(initRadius); //radius of 15
    accelFactor currAcceleration = noAccel;

    while(!WindowShouldClose()){
        currAcceleration = captureAccelerationInput(currAcceleration);
        processAccerelationInput(currAcceleration, player);
        renderPlayer(player);
    }

    CloseWindow();
    return 0;
}
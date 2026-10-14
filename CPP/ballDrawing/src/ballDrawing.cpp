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

/* 

*/

class ball{
    private:
        //positions
            

    public:
        ball(float r); //establishes the initial x and y position of the ball on the canvas
        ~ball(); //free it

        void updatePosition(int dX, int dY);

        //public member variables
        int x;
        int y;
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

void ball::updatePosition(int dX, int dY){
    this->x += dX;
    this->y += dY;
}

/* This will be split between 3 events: inputs, processing, then rendering.
*Inputs: the loop will handle obtaining the inputs then gives it to the ball class to do whatever it needs to.
This is to keep the ball class simple in that it should only worry about where the ball should be.

*Processing: This will handle keeping the ball in bounds of the window mainly. Perhaps acceleration too?
At the end of processing, it will give the processed inputs to the ball class

*Rendering: This will be the stuff between the begin and end drawing functions
*/

void captureInputs(){
    //handle key inputs and update the corresponding axis
    int key = GetKeyPressed(); //get the key that was pressed
    if(key == 0){ //0 means nothing was pressed
        return;
    }

    //valid inputs are w, a, s, d and the arrow key
    switch (key){
        //make the player go up
        case KEY_W:
        case KEY_UP:
            fprintf(stdout, "going up\n");
            break;

        //make the player go down
        case KEY_S:
        case KEY_DOWN:
            fprintf(stdout, "going down\n");
            break;

        //make the player go left
        case KEY_A:
        case KEY_LEFT:
            fprintf(stdout, "going left\n");
            break;
        
        //make the player go right
        case KEY_D:
        case KEY_RIGHT:
            fprintf(stdout, "going right\n");
            break;

        default:
            break;
    }
}

void renderPlayer(ball &player){
    //handle drawing the player in the window
    BeginDrawing();
    DrawCircle(player.x, player.y, player.radius, RED);
    EndDrawing();
}

int main(void){
    InitWindow(WIDTH, HEIGHT, "Ball Drawing Test");
    float initRadius = 15.0; //initial radius of the ball
    ball player = ball(initRadius); //radius of 15
    
    while(!WindowShouldClose()){
        captureInputs();
        renderPlayer(player);
    }

    CloseWindow();
    return 0;
}
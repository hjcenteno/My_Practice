/*
    author: Henry Centeno
    description:
        This prorgam is to successfully draw a circle to the screen and respond to user input
*/

#include <raylib.h>
#include <raymath.h>
#include <inttypes.h>
#include <stdio.h>

#define WIDTH (1200u)
#define HEIGHT (800u)
#define SPEED (500.0f)

constexpr float ACCELERATION = 1.0f;

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
    //handle key inputs and update the corresponding axis
    int key = GetKeyPressed(); //get the key that was pressed

    //valid inputs are w, a, s, d and the arrow key
    switch (key){
        //make the player go up
        case KEY_W:
        case KEY_UP:
            directions.y = -ACCELERATION;
            break;

        //make the player go down
        case KEY_S:
        case KEY_DOWN:
            directions.y = ACCELERATION;
            break;

        //make the player go left
        case KEY_A:
        case KEY_LEFT:
            directions.x = -ACCELERATION;
            break;
        
        //make the player go right
        case KEY_D:
        case KEY_RIGHT:
            directions.x = ACCELERATION;
            break;

        case KEY_SPACE: //stop moving
            directions.x = 0.0f;
            directions.y = 0.0f;
            break;
            
        default:
            break;
    }
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

int main(void){
    InitWindow(WIDTH, HEIGHT, "Ball Drawing Test");
    float initRadius = 15.0; //initial radius of the ball
    ball player = ball(initRadius); //radius of 15
    Vector2 currAcceleration = {0.0, 0.0};

    while(!WindowShouldClose()){
        captureAccelerationInput(currAcceleration);
        processAccerelationInput(currAcceleration, player);
        renderPlayer(player);
    }

    CloseWindow();
    return 0;
}
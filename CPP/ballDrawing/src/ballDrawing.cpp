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


int main(void){
    InitWindow(WIDTH, HEIGHT, "Ball Drawing Test");
    float initRadius = 15.0; //initial radius of the ball

    while(!WindowShouldClose()){
        BeginDrawing();
        ball player = ball(initRadius); //radius of 15
        DrawCircle(player.x, player.y, player.radius, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
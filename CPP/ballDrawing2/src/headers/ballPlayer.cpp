#include "ballPlayer.hpp"

BallPlayer::BallPlayer(float r){
    this->position = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    this->radius = r;
}

BallPlayer::~BallPlayer(){
}

void BallPlayer::updatePosition(float dx, float dy){
    //get the new position then see if it's in bounds of the current window
    float newX = this->position.x + dx;
    float newY = this->position.y + dy;

    //in bounds check
    bool inXBound = (newX > this->radius) && (newX < ((float)GetScreenWidth() - this->radius));
    bool inYBound = (newY > this->radius) && (newY < ((float)GetScreenHeight() - this->radius));
    if(inXBound && inYBound){
        this->position.x = newX;
        this->position.y = newY;
    }
}

void captureKeyInputs(Vector2 &accelerations){
    //determine the how the ball should accelerate
    float dx = 0.0f;
    float dy = 0.0f;

    if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)){
        dy = -BALL_ACCELERATION_RATE;
    }

    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){
        dy = BALL_ACCELERATION_RATE;
    }

    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
        dx = -BALL_ACCELERATION_RATE;
    }

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
        dx = BALL_ACCELERATION_RATE;
    }

    accelerations.x = dx;
    accelerations.y = dy;
}

void processAccelerationInput(Vector2 &accelerations, BallPlayer &player){
    const float step = GetFrameTime() * BALL_SPEED;
    accelerations = Vector2Normalize(accelerations); //makes the dx, dy between [0, 1]
    player.updatePosition((accelerations.x * step), (accelerations.y * step));
}
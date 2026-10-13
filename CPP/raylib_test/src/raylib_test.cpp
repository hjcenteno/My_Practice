/*
    author: Henry Centeno
    description:
        This program is to test if Raylib was succesfully installed in my machine
*/

#include "raylib.h"

int main(void){
    InitWindow(800, 800, "Test"); //one "global" context
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
        EndDrawing();
    }

    CloseWindow(); //closing that global context
}
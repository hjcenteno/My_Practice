#include "orientationHandling.hpp"

void captureSensorInputs(Vector2 &accelerations, std::atomic<orientation> &atomic_angles){
    orientation angles = atomic_angles.load();
    // printf("roll: %04f radians, pitch: %04f radians\n", angles.roll, angles.pitch);
    // printf("roll: %04f degrees, pitch: %04f degrees\n", RAD_TO_DEG(angles.roll), RAD_TO_DEG(angles.pitch));
    
    float dx = 0.0f;
    float dy = 0.0f;

    //don't roll/pitch until the degree exceeds the minimum
    if(MINIMUM_TILT_RADIANS < std::fabs(angles.roll)){
        // printf("Rolling at %f\n", RAD_TO_DEG(angles.roll));
        dx -= (BALL_ACCELERATION_RATE * angles.roll); //subtracting because of the axis of the mpu6050 
    }
    
    if(MINIMUM_TILT_RADIANS < std::fabs(angles.pitch)){
        // printf("Pitching at %f\n", RAD_TO_DEG(angles.pitch));
        dy += (BALL_ACCELERATION_RATE * angles.pitch); 
    }

    accelerations.x = dx;
    accelerations.y = dy;
}
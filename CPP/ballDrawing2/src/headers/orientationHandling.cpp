#include "orientationHandling.hpp"

void captureSensorInputs(Vector2 &accelerations, std::atomic<orientation> &atomic_angles){
    orientation angles = atomic_angles.load();
    printf("roll: %04f radians, pitch: %04f radians\n", angles.roll, angles.pitch);
    return;
}
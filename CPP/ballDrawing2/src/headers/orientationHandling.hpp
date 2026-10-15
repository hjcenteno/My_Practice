#ifndef ORIENTATIONHANDLING_HPP
#define ORIENTATIONHANDLING_HPP

#include "common.hpp"

#define RAD_TO_DEG(X) (X * RAD2DEG) //convert radians to degrees
#define DEG_TO_RAD(X) (X * DEG2RAD) //convert degree to radians

constexpr float MINIMUM_TILT_DEGREE = 8.0f; //the mininum degree of tilt to move the ball
constexpr float MINIMUM_TILT_RADIANS = DEG_TO_RAD(MINIMUM_TILT_DEGREE);

typedef struct orientation{
    float roll;
    float pitch;
}orientation;

/* Handles capturing the sensor inputs
 * Parameters:
 *   -Vector 2: reference to the vector holding the accelerations in the x and y axises
 */
void captureSensorInputs(Vector2 &accelerations, std::atomic<orientation> &atomic_angles);


#endif
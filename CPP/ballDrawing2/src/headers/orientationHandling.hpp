#ifndef ORIENTATIONHANDLING_HPP
#define ORIENTATIONHANDLING_HPP

#include "common.hpp"

typedef struct orientation{
    float roll;
    float pitch;
}orientation;

/* Handles capturing the sensor inputs
 * Parameters:
 *   -Vector 2: reference to the vector holding the accelerations in the x and y axises
 */
void captureSensorInputs(Vector2 &accelerations);


#endif
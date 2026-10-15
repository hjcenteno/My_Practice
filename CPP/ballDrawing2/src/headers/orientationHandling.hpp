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

/* The uart package handler to read data streamed by the microcontroller to dev/ttyacm0
 * Parameters:
 *   -const uint8_t *data: an array of bytes representing the raw bytes of the package
 *   -uint8_t length: number of bytes associated with the data
 */
void uart_angleHandler(const uint8_t *data, uint8_t length);

#endif
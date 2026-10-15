#ifndef BALLPLAYER_HPP
#define BALLPLAYER_HPP

#include "common.hpp"

class BallPlayer{
    public:
        BallPlayer(float r);
        ~BallPlayer();
        void updataPosition(float dx, float dy);

        //member variables
        Vector2 position;
        float radius;
};

/* Handles capturing the keyboard inputs
 * Parameters:
 *   -Vector 2: reference to the vector holding the accelerations in the x and y axises
 */
void captureKeyInputs(Vector2 &accelerations);

/* updates the positions of the player
 * Parameters:
 *   -Vector2 accelerations: vector holding the magnitudes of the acceleration in the x and y axises
 *   -ballPlayer player: reference to the player
 */
void processAccelerationInput(Vector2 &accelerations, BallPlayer &player);

#endif
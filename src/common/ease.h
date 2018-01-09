#ifndef FACE_SMASH_COMMON_EASE_H
#define FACE_SMASH_COMMON_EASE_H


#include <cmath>


namespace gamee {


float linear(float, float, float, float);
float easeInCubic(float, float, float, float);
float easeOutCubic(float, float, float, float);
float easeOutElastic(float, float, float, float);


}


#endif // FACE_SMASH_COMMON_EASE_H

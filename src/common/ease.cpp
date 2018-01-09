#include <cmath>
#include "ease.h"


namespace gamee {


float linear(float elapsed, float duration, float from, float to) {
    const auto t = elapsed/duration;
    return from + (to - from) * t;
}


float easeInCubic(float elapsed, float duration, float from, float to) {
    const auto t = elapsed/duration;
    return from + std::pow(t, 3) * (to - from);
}


float easeOutCubic(float elapsed, float duration, float from, float to) {
    const auto t = elapsed/duration;
    return from + (1 - std::pow(1 - t, 3)) * (to - from);
}


float easeOutElastic(float elapsed, float duration, float from, float to) {
    const auto t = elapsed/duration;
    const float p = .75f;
    return from + (std::pow(2, -10*t) * std::sin((t-p/4) * (2*3.14)/p) + 1) * (to - from);
}


}

#include <cmath>
#include "ease.h"


namespace gamee {


float linear(float elapsed, float duration, float from, float to) {
    const auto t = elapsed / duration;
    const auto remaining = to - from;
    return from + remaining * t;
}


float easeInCubic(float elapsed, float duration, float from, float to) {
    const auto t = elapsed / duration;
    const auto remaining = to - from;
    return from + std::pow(t, 3) * remaining;
}


float easeOutCubic(float elapsed, float duration, float from, float to) {
    const auto t = elapsed / duration;
    const auto remaining = to - from;
    return from + (1 - std::pow(1 - t, 3)) * remaining;
}


float easeInExpo(float elapsed, float duration, float from, float to) {
    const auto t = elapsed / duration;
    const auto remaining = to - from;
    return from + std::pow(2, 10 * (t - 1)) * remaining;
}


float easeOutExpo(float elapsed, float duration, float from, float to) {
    const auto t = elapsed / duration;
    const auto remaining = to - from;
    return from + (1 - std::pow(2, -10 * t)) * remaining;
}


float easeOutElastic(float elapsed, float duration, float from, float to) {
    const auto t = elapsed / duration;
    const auto remaining = to - from;
    const float p = .55f;
    return from + (std::pow(2, -10*t) * std::sin((t-p/4) * (2*3.14)/p) + 1) * remaining;
}


}

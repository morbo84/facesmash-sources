#ifndef FACE_SMASH_MATH_MATH_HPP
#define FACE_SMASH_MATH_MATH_HPP


#include <SDL_rect.h>
#include "../component/component.hpp"


namespace gamee {


namespace details {


template<typename Rect, typename Pos>
SDL_Rect operator *(const Rect &rect, const Pos &pos) {
    SDL_Rect result;

    result.x = rect.x + pos.x;
    result.y = rect.y + pos.y;
    result.w = rect.w;
    result.h = rect.h;

    return result;
}


template<typename Pos>
Pos operator +(const Pos &lhs, const Pos &rhs) {
    return Pos{ lhs.x + rhs.x, lhs.y + rhs.y };
}


template<typename Pos>
Pos operator -(const Pos &lhs, const Pos &rhs) {
    return Pos{ lhs.x - rhs.x, lhs.y - rhs.y };
}


}


inline float clamp(float value, float lim) {
    return (lim > 0 ? std::min(value, lim) : std::max(value, lim));
}


inline SDL_Rect operator *(const BoundingBox &box, const SDL_Point &position) {
    return details::operator *(box, position);
}


inline SDL_Rect operator *(const SDL_Point &position, const BoundingBox &box) {
    return details::operator *(box, position);
}


inline SDL_Point operator +(const SDL_Point &lhs, const SDL_Point &rhs) {
    return details::operator +(lhs, rhs);
}


inline SDL_Point operator -(const SDL_Point &lhs, const SDL_Point &rhs) {
    return details::operator -(lhs, rhs);
}


}


#endif // FACE_SMASH_MATH_MATH_HPP

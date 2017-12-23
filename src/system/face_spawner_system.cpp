#include "face_spawner_system.h"
#include <cmath>


namespace gamee {


struct Point {
    float x{};
    float y{};
};


class Ellipse {
    const float a; // horizontal semi-axis length
    const float b; // vertical semi-axis length
    const Point C; // center of the ellipse

public:
    Ellipse(float a, float b, Point center = {})
        : a{a}
        , b{b}
        , C{std::move(center)}
    {
        assert(a != 0.f && b != 0.f);
    }

    Point point(float theta) const {
        auto r = (b * b) / (a * (1 - std::cos(theta)));
        return {r * std::cos(theta) + C.x, r * std::sin(theta) + C.y};
    }
};


static const size_t width = 1920U;
static const size_t height = 1080U;
static float percent = 0.2f;
static Ellipse comfortRegion{percent * width, percent * height, {0.5f * width, 0.5f * height}};


static float randomFloat(float a, float b) {
    auto random = rand() / static_cast<float>(RAND_MAX);
    auto r = random * (b - a);
    return a + r;
}


static float randomTheta() {
    constexpr auto twoPi = 2 * M_PI;
    return randomFloat(0, twoPi);
}


enum class Edge {top, left, bottom, right};



Point startingPoint() {
    Point ret;
    auto edge = static_cast<Edge>(rand() % 4);
    switch (edge) {
    case Edge::top:
    case Edge::bottom:
        ret.x = randomFloat(0, width);
        ret.y = edge == Edge::top ? 0.f : static_cast<float>(height);
        break;
    case Edge::left:
    case Edge::right:
        ret.x = edge == Edge::left ? 0.f : static_cast<float>(width);
        ret.y = randomFloat(0, height);
        break;
    }
    return ret;
}


Parabola FaceSpawnerSystem::spawnPath() const {
    constexpr auto g = 1.f; // TODO: find a reasonable value
    auto p1 = startingPoint();
    auto p2 = comfortRegion.point(randomTheta());
    auto vy = randomFloat(1.f, 2.f); // TODO: find reasonable values
    auto t2 = (-vy + std::sqrt(vy * vy - 2 * g * (p1.y - p2.y))) / g;
    auto vx = (p2.x - p1.x) / t2;
    return {g, {p1.x, p1.y, 0.f}, vx, vy};
}


void FaceSpawnerSystem::update(Registry& registry, delta_type delta) {

}


}

#include <cmath>
#include <cassert>
#include <SDL_rect.h>
#include "../common/constants.h"
#include "../locator/locator.hpp"
#include "face_spawner_system.h"


namespace gamee {


class Ellipse {
    const float a; // horizontal semi-axis length
    const float b; // vertical semi-axis length
    const Transform C; // center of the ellipse

public:
    Ellipse(float a, float b, Transform center = {})
        : a{a}
        , b{b}
        , C{std::move(center)}
    {
        assert(a != 0.f && b != 0.f);
    }

    Transform point(float theta) const {
        auto r = (a*b) / (std::sqrt(std::pow((b*std::cos(theta)), 2) + std::pow((a*std::sin(theta)), 2)));

        Transform ret;
        ret.x = r * std::cos(theta) + C.x;
        ret.y = r * std::sin(theta) + C.y;

        return ret;
    }
};


static float randomFloat(float a, float b) {
    auto random = rand() / static_cast<float>(RAND_MAX);
    auto r = random * (b - a);
    return a + r;
}


static float randomTheta() {
    constexpr auto twoPi = 2 * M_PI;
    return randomFloat(0, twoPi);
}


Transform startingPoint() {
    const float y = logicalHeight;
    constexpr float range = 200.f;
    auto x = randomFloat(-range, logicalWidth + range);
    return {x, y};
}


std::pair<Transform, Movement> FaceSpawnerSystem::spawnPath() const {
    constexpr auto gravity = 0.001f; // TODO: find a reasonable value
    auto p1 = startingPoint();
    Ellipse comfortRegion{.2f * logicalWidth, .2f * logicalHeight, {.5f * logicalWidth, .5f * logicalHeight}};
    auto p2 = comfortRegion.point(randomTheta());
    auto t = randomFloat(750.f, 1500.f); // TODO: find reasonable values
    auto velX = (p2.x - p1.x) / t;
    auto velY = -t * gravity;
    return {p1, {gravity, velX, velY}};
}


static entt::HashedString emoji() {
    auto type = rand() % (SmashType::SAD + 1);

    switch (type) {
    case SmashType::ANGRY:
        return "emoji/angry";
    case SmashType::DISGUSTED:
        return "emoji/disgusted";
    case SmashType::HAPPY:
        return "emoji/happy";
    case SmashType::RESTED:
        return "emoji/rested";
    case SmashType::SURPRISED:
        return "emoji/surprised";
    case SmashType::FEARFUL:
        return "emoji/fearful";
    case SmashType::SAD:
        return "emoji/sad";
    default:
        assert(false);
        (void)0;
    }
}


void FaceSpawnerSystem::update(Registry& registry, delta_type delta) {
    elapsed += delta;

    if(elapsed > interval) {
        auto &textureCache = Locator::TextureCache::ref();
        auto path = spawnPath();
        auto entity = registry.create();

        registry.assign<Transform>(entity, 1.f * path.first.x, 1.f * path.first.y);
        registry.assign<Renderable>(entity);
        registry.assign<Sprite>(entity, textureCache.handle(emoji()), 128, 128, 64, 64);
        registry.assign<BoundingBox>(entity, 64, 64);
        registry.assign<Movement>(entity, path.second);

        elapsed = delta_type{};
    }
}


}

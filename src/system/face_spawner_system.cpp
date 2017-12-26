#include <cmath>
#include <cassert>
#include <SDL_rect.h>
#include "../locator/locator.hpp"
#include "../settings/settings.h"
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
    Settings settings;
    float y = settings.logicalHeight();
    constexpr float range = 200.f;
    auto x = randomFloat(-range, settings.logicalWidth() + range);
    return {x, y};
}


std::pair<Transform, Parabola> FaceSpawnerSystem::spawnPath() const {
    constexpr auto g = 0.001f; // TODO: find a reasonable value
    auto p1 = startingPoint();
    Settings settings;
    Ellipse comfortRegion{.2f * settings.logicalWidth(), .2f * settings.logicalHeight(), {.5f * settings.logicalWidth(), .5f * settings.logicalHeight()}};
    auto p2 = comfortRegion.point(randomTheta());
    auto t = randomFloat(750.f, 1500.f); // TODO: find reasonable values
    auto vx = (p2.x - p1.x) / t;
    auto vy = -t * g;
    return {p1, {g, vx, vy}};
}


enum class Emotion {
    angry, disgusted, happy, rested, surprised, fearful, sad
};


static entt::HashedString emoji() {
    auto r = rand() % (static_cast<int>(Emotion::sad) + 1);
    switch (static_cast<Emotion>(r)) {
    case Emotion::angry:
        return "emoji/angry";
    case Emotion::disgusted:
        return "emoji/disgusted";
    case Emotion::happy:
        return "emoji/happy";
    case Emotion::rested:
        return "emoji/rested";
    case Emotion::surprised:
        return "emoji/surprised";
    case Emotion::fearful:
        return "emoji/fearful";
    case Emotion::sad:
        return "emoji/sad";
    default:
        assert(0);
        return "emoji/happy";
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
        registry.assign<Sprite>(entity, textureCache.handle(emoji()), 128_ui16, 128_ui16, 64_ui16, 64_ui16);
        registry.assign<Parabola>(entity, path.second);

        elapsed = delta_type{};
    }
}


}

#include "../process/process_init.h"
#include "../process/process_splash.h"
#include "scene_splash.h"


namespace gamee {


void SceneSplash::update(GameRenderer &renderer, delta_type delta) {
    scheduler.update(delta, &renderer);
}


void SceneSplash::entering() {
    scheduler.attach<ProcessInit>().then<ProcessSplash>();
}


void SceneSplash::leaving() {
    scheduler.abort(true);
    scheduler.clear();
}


}

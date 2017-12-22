#include <SDL.h>
#include <memory>
#include "game/game_loop.h"
#include "input/user_input_handler.h"
#include "locator/locator.hpp"
#include "types/types.hpp"


int main(int , char **) {
    // set up services
    gamee::Locator::TTFFontCache::set();
    gamee::Locator::TextureCache::set();
    gamee::Locator::Dispatcher::set();
    gamee::Locator::InputHandler::set<gamee::UserInputHandler>();

    // create a new game loop and initialize the environment
    auto loop = std::make_unique<gamee::GameLoop>();

    // enjoy!! :-)
    auto ret = loop->exec();

    // destroy the loop
    loop.reset();

    // tear down services
    gamee::Locator::InputHandler::reset();
    gamee::Locator::Dispatcher::reset();
    gamee::Locator::TextureCache::reset();
    gamee::Locator::TTFFontCache::reset();

    // bye bye, have a nice day
    return ret;
}

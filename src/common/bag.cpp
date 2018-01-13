#include "bag.h"


namespace gamee {


FaceBag::FaceBag()
    : generator{std::random_device{}()},
      buffer{
          FaceType::ANGRY,
          FaceType::DISGUSTED,
          FaceType::FEARFUL,
          FaceType::HAPPY,
          FaceType::SAD,
          FaceType::SURPRISED
      },
      next{numberOfFaces}
{}


FaceType FaceBag::get() {
    if(next == numberOfFaces) {
        std::shuffle(buffer, buffer+numberOfFaces, generator);
        next = 0_ui8;
    }

    return buffer[next++];
}


ItemBag::ItemBag()
    : generator{std::random_device{}()},
      buffer{
          ItemType::FOUNTAIN,
          ItemType::SLOW_DOWN,
          ItemType::SPEED_UP
      },
      next{numberOfItems}
{}


ItemType ItemBag::get() {
    if(next == numberOfItems) {
        std::shuffle(buffer, buffer+numberOfItems, generator);
        next = 0_ui8;
    }

    return buffer[next++];
}


}

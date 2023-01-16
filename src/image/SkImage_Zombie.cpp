#include "src/image/SkImage_Zombie.h"

sk_sp<SkImage> SkImage::MakeZombie(sk_sp<SkImage>&& image) {
  if (image) {
    return sk_make_sp<SkZombieImage>(std::move(image));
  } else {
    return nullptr;
  }
}

#include "include/core/SkImage.h"
#include "src/image/SkImage_Base.h"
#include "include/private/SkMacros.h"
#include "src/core/SkNextID.h"
#include "Zombie/zombie.hpp"

struct SkZombieImage : SkImage_Base {
  Zombie<sk_sp<SkImage>> image;
  // maybe call SkNextID::ImageID()?
 SkZombieImage(sk_sp<SkImage>&& image) :
   SkImage_Base(image->imageInfo(), image->uniqueID()),
   image([&](){
       SkASSERT(image->unique());
       return std::move(image);
     }()) {
  }

  bool isZombie() const override {
    return true;
  }

  const SkZombieImage* asZombie() const override {
    return this;
  }

  bool onReadPixels(GrDirectContext*, const SkImageInfo&, void*, size_t, int, int, SkImage::CachingHint) const override {
    AccessForbidden();
  }

  bool onHasMipmaps() const override {
    AccessForbidden();
  }

  bool getROPixels(GrDirectContext*, SkBitmap*, SkImage::CachingHint) const override {
    AccessForbidden();
  }

  sk_sp<SkImage> onMakeSubset(const SkIRect&, GrDirectContext*) const override {
    AccessForbidden();
  }

  bool onIsValid(GrRecordingContext*) const override {
    AccessForbidden();
  }

  sk_sp<SkImage> onMakeColorTypeAndColorSpace(SkColorType, sk_sp<SkColorSpace>, GrDirectContext*) const override {
    AccessForbidden();
  }

  sk_sp<SkImage> onReinterpretColorSpace(sk_sp<SkColorSpace>) const override {
    AccessForbidden();
  }

  std::tuple<GrSurfaceProxyView, GrColorType> onAsView(GrRecordingContext*, GrMipmapped, GrImageTexGenPolicy) const override {
    AccessForbidden();
  }

  std::unique_ptr<GrFragmentProcessor> onAsFragmentProcessor(GrRecordingContext*, SkSamplingOptions, const SkTileMode*, const SkMatrix&, const SkRect*, const SkRect*) const override {
    AccessForbidden();
  }
};

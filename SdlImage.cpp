#include "SdlImage.hpp"

#include <SDL2/SDL_image.h>

#include <cstdint>
#include <stdexcept>

namespace xsecurelock_saver_slide {
namespace {
using std::runtime_error;
using std::uint32_t;
}  // namespace

SdlImage::SdlImage() {
  uint32_t img_flags = IMG_INIT_PNG;
  if ((static_cast<uint32_t>(IMG_Init(img_flags)) & img_flags) != img_flags) {
    throw runtime_error{"Error calling IMG_Init"};
  }
}

SdlImage::~SdlImage() { IMG_Quit(); }
}  // namespace xsecurelock_saver_slide

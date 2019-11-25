#include "SdlTtf.hpp"

#include <SDL2/SDL_ttf.h>

#include <stdexcept>

namespace xsecurelock_saver_slide {
namespace {
using std::runtime_error;
}  // namespace

SdlTtf::SdlTtf() {
  if (TTF_Init() != 0) {
    throw runtime_error{"Error calling TTF_Init"};
  }
}

SdlTtf::~SdlTtf() { TTF_Quit(); }
}  // namespace xsecurelock_saver_slide

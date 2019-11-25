#include "SdlApp.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>
#include <string>

namespace xsecurelock_saver_slide {
namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

SdlApp::SdlApp() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw runtime_error{"Error calling SDL_Init: "s + SDL_GetError()};
  }
}

SdlApp::~SdlApp() { SDL_Quit(); }
}  // namespace xsecurelock_saver_slide

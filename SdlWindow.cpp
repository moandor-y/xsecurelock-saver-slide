#include "SdlWindow.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>

namespace xsecurelock_saver_slide {
namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

SdlWindow::SdlWindow(const void* data) : window_{SDL_CreateWindowFrom(data)} {
  if (window_ == nullptr) {
    throw runtime_error{"Error calling SDL_CreateWindowFrom: "s +
                        SDL_GetError()};
  }
}

SdlWindow::SdlWindow(const std::string& title, int x, int y, int w, int h,
                     std::uint32_t flags)
    : window_{SDL_CreateWindow(title.c_str(), x, y, w, h, flags)} {
  if (window_ == nullptr) {
    throw runtime_error{"Error calling SDL_CreateWindow: "s + SDL_GetError()};
  }
}

void SdlWindow::Deleter::operator()(SDL_Window* window) const noexcept {
  SDL_DestroyWindow(window);
}
}  // namespace xsecurelock_saver_slide

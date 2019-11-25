#include "SdlRwOps.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>
#include <string>

namespace xsecurelock_saver_slide {
namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

SdlRwOps::SdlRwOps(const void* mem, int size)
    : rw_{SDL_RWFromConstMem(mem, size)} {
  if (rw_ == nullptr) {
    throw runtime_error{"Error calling SDL_RWFromConstMem: "s + SDL_GetError()};
  }
}

SdlRwOps::SdlRwOps(const std::string& file, const std::string& mode)
    : rw_{SDL_RWFromFile(file.c_str(), mode.c_str())} {
  if (rw_ == nullptr) {
    throw runtime_error{"Error calling SDL_RWFromFile: "s + SDL_GetError()};
  }
}

void SdlRwOps::Deleter::operator()(SDL_RWops* ops) const noexcept {
  SDL_RWclose(ops);
}
}  // namespace xsecurelock_saver_slide

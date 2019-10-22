#include "SdlSurface.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdexcept>
#include <string>

#include "SdlRwOps.hpp"
#include "SdlTtfFont.hpp"

namespace xsecurelock_saver_slide {
namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

SdlSurface::SdlSurface(xsecurelock_saver_slide::SdlTtfFont& font,
                       const std::string& text, SDL_Color fg)
    : surface_{TTF_RenderText_Blended(font.get(), text.c_str(), fg)} {
  if (surface_ == nullptr) {
    throw runtime_error{"Error calling TTF_RenderText_Blended: "s +
                        TTF_GetError()};
  }
}

SdlSurface::SdlSurface(SdlRwOps& ops) : surface_{SDL_LoadBMP_RW(ops.get(), 0)} {
  if (surface_ == nullptr) {
    throw runtime_error{"Error calling SDL_LoadBMP_RW: "s + SDL_GetError()};
  }
}

SdlSurface SdlSurface::ConvertSurfaceFormat(std::uint32_t format) {
  SDL_Surface* surface = SDL_ConvertSurfaceFormat(surface_.get(), format, 0);
  if (surface_ == nullptr) {
    throw runtime_error{"Error calling SDL_ConvertSurfaceFormat: "s +
                        SDL_GetError()};
  }
  SdlSurface result{};
  result.surface_.reset(surface);
  return result;
}

void SdlSurface::Lock() {
  if (SDL_LockSurface(surface_.get()) != 0) {
    throw runtime_error{"Error calling SDL_LockSurface: "s + SDL_GetError()};
  }
}

void SdlSurface::Unlock() noexcept { SDL_UnlockSurface(surface_.get()); }

bool SdlSurface::MustLock() const noexcept {
  return SDL_MUSTLOCK(surface_.get());
}

SdlSurface::SdlSurface(void* pixels, int width, int height, int depth,
                       int pitch, std::uint32_t format)
    : surface_{SDL_CreateRGBSurfaceWithFormatFrom(pixels, width, height, depth,
                                                  pitch, format)} {
  if (surface_ == nullptr) {
    throw runtime_error{"Error calling SDL_CreateRGBSurfaceWithFormatFrom: "s +
                        SDL_GetError()};
  }
}

SdlSurface::SdlSurface(int width, int height, int depth, std::uint32_t format)
    : surface_{
          SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, format)} {
  if (surface_ == nullptr) {
    throw runtime_error{"Error calling SDL_CreateRGBSurfaceWithFormat: "s +
                        SDL_GetError()};
  }
}

SdlSurface::SdlSurface(SdlRwOps&& ops) : SdlSurface(ops) {}

void SdlSurface::Deleter::operator()(SDL_Surface* surface) const noexcept {
  SDL_FreeSurface(surface);
}
}  // namespace xsecurelock_saver_slide

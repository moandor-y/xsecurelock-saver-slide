#include "SdlTexture.hpp"

#include <stdexcept>

#include "SdlRenderer.hpp"
#include "SdlSurface.hpp"

namespace xsecurelock_saver_slide {
namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

SdlTexture::Attributes SdlTexture::QueryTexture() {
  Attributes attributes{};
  if (SDL_QueryTexture(texture_.get(), &attributes.format, &attributes.access,
                       &attributes.width, &attributes.height) != 0) {
    throw runtime_error{"Error calling SDL_QueryTexture: "s + SDL_GetError()};
  }
  return attributes;
}

SdlTexture::SdlTexture(xsecurelock_saver_slide::SdlRenderer& renderer,
                       xsecurelock_saver_slide::SdlSurface& surface)
    : texture_{SDL_CreateTextureFromSurface(renderer.get(), surface.get())} {
  if (texture_ == nullptr) {
    throw runtime_error{"Error calling SDL_CreateTextureFromSurface: "s +
                        SDL_GetError()};
  }
}

void SdlTexture::SetTextureAlphaMod(std::uint8_t alpha) {
  if (SDL_SetTextureAlphaMod(texture_.get(), alpha) != 0) {
    throw runtime_error{"Error calling SDL_SetTextureAlphaMod: "s +
                        SDL_GetError()};
  }
}

void SdlTexture::SetTextureBlendMode(SDL_BlendMode mode) {
  if (SDL_SetTextureBlendMode(texture_.get(), mode) != 0) {
    throw runtime_error{"Error calling SDL_SetTextureBlendMode: "s +
                        SDL_GetError()};
  }
}

void SdlTexture::Deleter::operator()(SDL_Texture* texture) const noexcept {
  SDL_DestroyTexture(texture);
}

SdlTexture::SdlTexture(xsecurelock_saver_slide::SdlRenderer& renderer,
                       std::uint32_t format, int access, int width, int height)
    : texture_{
          SDL_CreateTexture(renderer.get(), format, access, width, height)} {
  if (texture_ == nullptr) {
    throw runtime_error{"Error calling SDL_CreateTexture: "s + SDL_GetError()};
  }
}
}  // namespace xsecurelock_saver_slide

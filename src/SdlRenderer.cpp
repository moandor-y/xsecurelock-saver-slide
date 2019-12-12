#include "SdlRenderer.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>

#include "SdlTexture.hpp"
#include "SdlWindow.hpp"

namespace xsecurelock_saver_slide {
namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

void SdlRenderer::SetRenderDrawColor(Color color) {
  if (SDL_SetRenderDrawColor(renderer_.get(), color.r, color.g, color.b,
                             color.a) != 0) {
    throw runtime_error{"Error calling SDL_SetRenderDrawColor: "s +
                        SDL_GetError()};
  }
}

SdlRenderer::SdlRenderer(xsecurelock_saver_slide::SdlWindow& window, int index,
                         std::uint32_t flags)
    : renderer_{SDL_CreateRenderer(window.get(), index, flags)} {
  if (renderer_ == nullptr) {
    throw runtime_error{"Error calling SDL_CreateRenderer: "s + SDL_GetError()};
  }
}

void SdlRenderer::RenderClear() {
  if (SDL_RenderClear(renderer_.get()) != 0) {
    throw runtime_error{"Error calling SDL_RenderClear: "s + SDL_GetError()};
  }
}

void SdlRenderer::RenderCopy(SdlTexture& texture, const SDL_Rect* srcrect,
                             const SDL_Rect* dstrect) {
  if (SDL_RenderCopy(renderer_.get(), texture.get(), srcrect, dstrect) != 0) {
    throw runtime_error{"Error calling SDL_RenderCopy: "s + SDL_GetError()};
  }
}

void SdlRenderer::RenderPresent() { SDL_RenderPresent(renderer_.get()); }

SDL_RendererInfo SdlRenderer::GetRendererInfo() const {
  SDL_RendererInfo info{};
  if (SDL_GetRendererInfo(renderer_.get(), &info) != 0) {
    throw runtime_error{"Error calling SDL_GetRendererInfo: "s +
                        SDL_GetError()};
  }
  return info;
}

SdlRenderer::OutputSize SdlRenderer::GetRendererOutputSize() {
  OutputSize size{};
  if (SDL_GetRendererOutputSize(renderer_.get(), &size.width, &size.height) !=
      0) {
    throw runtime_error{"Error calling SDL_GetRendererOutputSize: "s +
                        SDL_GetError()};
  }
  return size;
}

void SdlRenderer::Deleter::operator()(SDL_Renderer* renderer) const noexcept {
  SDL_DestroyRenderer(renderer);
}

SDL_Texture* SdlRenderer::GetRenderTarget() const noexcept {
  return SDL_GetRenderTarget(renderer_.get());
}

void SdlRenderer::SetRenderTarget(SDL_Texture* texture) {
  if (SDL_SetRenderTarget(renderer_.get(), texture) != 0) {
    throw runtime_error{"Error calling SDL_SetRenderTarget: "s +
                        SDL_GetError()};
  }
}

SdlRenderer::Color SdlRenderer::GetRenderDrawColor() const {
  Color result{};
  if (SDL_GetRenderDrawColor(renderer_.get(), &result.r, &result.g, &result.b,
                             &result.a) != 0) {
    throw runtime_error{"Error calling SDL_GetRenderDrawColor: "s +
                        SDL_GetError()};
  }
  return result;
}
}  // namespace xsecurelock_saver_slide

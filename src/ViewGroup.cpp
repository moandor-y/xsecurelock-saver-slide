#include "ViewGroup.hpp"

#include <cstdint>

#include <SDL2/SDL.h>

#include "Application.hpp"
#include "SdlRenderer.hpp"

namespace xsecurelock_saver_slide {
namespace {
using std::uint32_t;
}

void ViewGroup::Draw(SdlRenderer& renderer) {
  {
    struct RenderTargetGuard {
      SdlRenderer& renderer;
      SDL_Texture* texture;
      ~RenderTargetGuard() { renderer.SetRenderTarget(texture); }
    };
    RenderTargetGuard target_guard{renderer, renderer.GetRenderTarget()};
    renderer.SetRenderTarget(texture().get());

    struct RenderColorGuard {
      SdlRenderer& renderer;
      SdlRenderer::Color color;
      ~RenderColorGuard() { renderer.SetRenderDrawColor(color); }
    };
    RenderColorGuard color_guard{renderer, renderer.GetRenderDrawColor()};
    renderer.SetRenderDrawColor({0, 0, 0, 0});

    renderer.RenderClear();

    for (const auto& child : children_) {
      child->Draw(renderer);
    }
  }

  TextureView::Draw(renderer);
}

SdlTexture ViewGroup::CreateTexture(SdlRenderer& renderer) {
  int width = right() - left();
  int height = bottom() - top();
  return SdlTexture{renderer, Application::GetPixelFormat(),
                    SDL_TEXTUREACCESS_TARGET, width, height};
}
}  // namespace xsecurelock_saver_slide

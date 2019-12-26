#include "ViewGroup.hpp"

#include <SDL2/SDL.h>

#include <memory>
#include <utility>

#include "Application.hpp"
#include "SdlRenderer.hpp"

namespace xsecurelock_saver_slide {
namespace {
using std::move;
using std::uint32_t;
using std::unique_ptr;
}  // namespace

void ViewGroup::OnDraw() {
  {
    struct RenderTargetGuard {
      SdlRenderer& renderer;
      SDL_Texture* texture;
      ~RenderTargetGuard() { renderer.SetRenderTarget(texture); }
    };
    RenderTargetGuard target_guard{renderer(), renderer().GetRenderTarget()};
    renderer().SetRenderTarget(texture().get());

    struct RenderColorGuard {
      SdlRenderer& renderer;
      SdlRenderer::Color color;
      ~RenderColorGuard() { renderer.SetRenderDrawColor(color); }
    };
    RenderColorGuard color_guard{renderer(), renderer().GetRenderDrawColor()};
    renderer().SetRenderDrawColor({0, 0, 0, 0});

    renderer().RenderClear();

    for (const auto& child : children_) {
      child->Draw();
    }
  }

  TextureView::OnDraw();
}

SdlTexture ViewGroup::CreateTexture() {
  int width = right() - left();
  int height = bottom() - top();
  SdlTexture texture{renderer(), Application::GetPixelFormat(),
                     SDL_TEXTUREACCESS_TARGET, width, height};
  texture.SetTextureBlendMode(SDL_BLENDMODE_BLEND);
  return texture;
}

void ViewGroup::AddChild(unique_ptr<View> view) {
  children_.push_back(move(view));
}
}  // namespace xsecurelock_saver_slide

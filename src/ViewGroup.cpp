#include "ViewGroup.hpp"

namespace xsecurelock_saver_slide {
void ViewGroup::Draw(SdlRenderer& renderer) {
  struct RenderTargetGuard {
    SdlRenderer& renderer;
    SDL_Texture* texture;
    ~RenderTargetGuard() { renderer.SetRenderTarget(texture); }
  };

  SDL_Texture* original_target = renderer.GetRenderTarget();
  renderer.SetRenderTarget(texture_.get());
  RenderTargetGuard guard{renderer, original_target};

  for (const auto& view : views_) {
    view->Draw(renderer);
  }

  // TODO texture draw
}
}  // namespace xsecurelock_saver_slide

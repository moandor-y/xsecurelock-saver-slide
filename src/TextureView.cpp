#include "TextureView.hpp"

#include "SdlRenderer.hpp"

namespace xsecurelock_saver_slide {
void TextureView::Draw(SdlRenderer& renderer) {
  if (alpha_dirty_) {
    texture_.SetTextureAlphaMod(lround(alpha_ * 255));
    alpha_dirty_ = false;
  }

  SDL_Rect rect{};
  rect.x = left();
  rect.y = top();
  rect.w = right() - left();
  rect.h = bottom() - top();

  renderer.RenderCopy(texture_, nullptr, &rect);
}

void TextureView::SetAlpha(double value) {
  if (value < 0) {
    value = 0;
  } else if (value > 1) {
    value = 1;
  }

  if (value != alpha_) {
    alpha_ = value;
    alpha_dirty_ = true;
  }
}

void TextureView::OnPreDraw(SdlRenderer& renderer) {
  if (!texture_.valid() || texture_dirty_) {
    texture_ = CreateTexture(renderer);
    texture_dirty_ = false;
  }
}
}  // namespace xsecurelock_saver_slide

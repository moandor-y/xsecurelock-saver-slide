#include "TextureView.hpp"

#include "SdlRenderer.hpp"

namespace xsecurelock_saver_slide {
void TextureView::OnDraw() {
  SDL_Rect rect{};
  rect.x = left();
  rect.y = top();
  rect.w = right() - left();
  rect.h = bottom() - top();

  renderer().RenderCopy(texture_, nullptr, &rect);
}

void TextureView::OnPreDraw() {
  int width = right() - left();
  int height = bottom() - top();
  if (!texture_.valid() || texture_width_ != width ||
      texture_height_ != height) {
    texture_ = CreateTexture();
    texture_width_ = width;
    texture_height_ = height;
  }

  if (texture_alpha_ != alpha()) {
    texture_.SetTextureAlphaMod(lround(alpha() * 255));
    texture_alpha_ = alpha();
  }
}
}  // namespace xsecurelock_saver_slide

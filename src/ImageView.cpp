#include "ImageView.hpp"

#include "SdlRenderer.hpp"

namespace {
using std::move;
}

namespace xsecurelock_saver_slide {
ImageView::ImageView(SdlRenderer& renderer, SdlSurface surface)
    : texture_{renderer, surface} {
  texture_.SetTextureBlendMode(SDL_BLENDMODE_BLEND);
}

void ImageView::Draw(SdlRenderer& renderer) {
  SDL_Rect rect{};
  rect.x = left();
  rect.y = top();
  rect.w = right() - left();
  rect.h = bottom() - top();

  renderer.RenderCopy(texture_, nullptr, &rect);
}

double ImageView::GetAlpha() const { return alpha_; }

void ImageView::SetAlpha(double value) {
  if (value < 0) {
    value = 0;
  } else if (value > 1) {
    value = 1;
  }
  alpha_ = value;

  texture_.SetTextureAlphaMod(lround(alpha_ * 255));
}
}  // namespace xsecurelock_saver_slide

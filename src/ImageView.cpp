#include "ImageView.hpp"

#include "SdlRenderer.hpp"

namespace xsecurelock_saver_slide {
namespace {
using std::min;
using std::move;
}  // namespace

ImageView::ImageView(SdlRenderer& renderer, SdlSurface& surface)
    : View(renderer), texture_{renderer, surface} {
  surface_width_ = surface.width();
  surface_height_ = surface.height();
  texture_.SetTextureBlendMode(SDL_BLENDMODE_BLEND);
}

void ImageView::OnDraw() {
  if (!texture_.valid()) {
    return;
  }

  SDL_Rect rect{};
  rect.x = left();
  rect.y = top();
  rect.w = right() - left();
  rect.h = bottom() - top();

  renderer().RenderCopy(texture_, nullptr, &rect);
}

void ImageView::SetImage(SdlSurface& surface) {
  texture_ = SdlTexture{renderer(), surface};
  surface_width_ = surface.width();
  surface_height_ = surface.height();
}

void ImageView::OnPreDraw() {
  if (texture_alpha_ != alpha()) {
    texture_.SetTextureAlphaMod(lround(alpha() * 255));
    texture_alpha_ = alpha();
  }
}
}  // namespace xsecurelock_saver_slide

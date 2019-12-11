#ifndef XSECURELOCK_SAVER_SLIDE_IMAGE_VIEW_HPP_
#define XSECURELOCK_SAVER_SLIDE_IMAGE_VIEW_HPP_

#include "SdlSurface.hpp"
#include "SdlTexture.hpp"
#include "View.hpp"

namespace xsecurelock_saver_slide {
class SdlRenderer;

class ImageView : public View {
 public:
  explicit ImageView(SdlRenderer&, SdlSurface);
  void Draw(SdlRenderer&) override;
  [[nodiscard]] double GetAlpha() const override;
  void SetAlpha(double) override;

 private:
  SdlTexture texture_;
  double alpha_ = 1;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_IMAGE_VIEW_HPP_

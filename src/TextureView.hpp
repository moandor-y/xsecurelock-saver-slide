#ifndef XSECURELOCK_SAVER_SLIDE_TEXTURE_VIEW_HPP_
#define XSECURELOCK_SAVER_SLIDE_TEXTURE_VIEW_HPP_

#include <utility>

#include "SdlTexture.hpp"
#include "View.hpp"

namespace xsecurelock_saver_slide {
class TextureView : public View {
 public:
  void OnPreDraw(SdlRenderer&) override;
  void Draw(SdlRenderer&) override;
  void SetAlpha(double) override;

  [[nodiscard]] double alpha() const { return alpha_; }

 protected:
  SdlTexture& texture() { return texture_; }

 private:
  SdlTexture texture_;
  double alpha_ = 1;
  bool alpha_dirty_ = false;
  bool texture_dirty_ = false;

  virtual SdlTexture CreateTexture(SdlRenderer&) = 0;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_TEXTURE_VIEW_HPP_

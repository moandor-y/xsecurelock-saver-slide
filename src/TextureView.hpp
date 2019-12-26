#ifndef XSECURELOCK_SAVER_SLIDE_TEXTURE_VIEW_HPP_
#define XSECURELOCK_SAVER_SLIDE_TEXTURE_VIEW_HPP_

#include <utility>

#include "SdlTexture.hpp"
#include "View.hpp"

namespace xsecurelock_saver_slide {
class TextureView : public View {
 public:
  using View::View;

 protected:
  SdlTexture& texture() { return texture_; }

  void OnDraw() override;
  void OnPreDraw() override;

 private:
  SdlTexture texture_;
  double texture_alpha_{};
  int texture_width_{};
  int texture_height_{};

  virtual SdlTexture CreateTexture() = 0;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_TEXTURE_VIEW_HPP_

#ifndef XSECURELOCK_SAVER_SLIDE_VIEW_GROUP_HPP_
#define XSECURELOCK_SAVER_SLIDE_VIEW_GROUP_HPP_

#include <memory>
#include <vector>

#include "SdlTexture.hpp"
#include "TextureView.hpp"

namespace xsecurelock_saver_slide {
class ViewGroup : public TextureView {
 public:
  void Draw(SdlRenderer&) override;
  SdlTexture CreateTexture(SdlRenderer&) override;

 private:
  std::vector<std::unique_ptr<View>> children_;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_VIEW_GROUP_HPP_

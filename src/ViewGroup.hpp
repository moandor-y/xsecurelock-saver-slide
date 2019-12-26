#ifndef XSECURELOCK_SAVER_SLIDE_VIEW_GROUP_HPP_
#define XSECURELOCK_SAVER_SLIDE_VIEW_GROUP_HPP_

#include <memory>
#include <vector>

#include "SdlTexture.hpp"
#include "TextureView.hpp"

namespace xsecurelock_saver_slide {
class ViewGroup : public TextureView {
 public:
  using TextureView::TextureView;
  void AddChild(std::unique_ptr<View>);

 protected:
  void OnDraw() override;

 private:
  std::vector<std::unique_ptr<View>> children_;

  SdlTexture CreateTexture() override;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_VIEW_GROUP_HPP_

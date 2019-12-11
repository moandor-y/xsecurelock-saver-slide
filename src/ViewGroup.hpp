#ifndef XSECURELOCK_SAVER_SLIDE_VIEW_GROUP_HPP_
#define XSECURELOCK_SAVER_SLIDE_VIEW_GROUP_HPP_

#include <memory>
#include <vector>

#include "SdlRenderer.hpp"
#include "SdlTexture.hpp"
#include "View.hpp"

namespace xsecurelock_saver_slide {
class ViewGroup : public View {
 public:
  void Draw(SdlRenderer&) override;
  [[nodiscard]] double GetAlpha() const override;
  void SetAlpha(double) override;

 private:
  SdlTexture texture_;
  double alpha_ = 1;
  std::vector<std::unique_ptr<View>> views_;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_VIEW_GROUP_HPP_

#ifndef XSECURELOCK_SAVER_SLIDE_VIEW_HPP_
#define XSECURELOCK_SAVER_SLIDE_VIEW_HPP_

namespace xsecurelock_saver_slide {
class SdlRenderer;

class View {
 public:
  explicit View(SdlRenderer& renderer) : renderer_{renderer} {};
  View(const View&) = delete;
  View& operator=(const View&) = delete;
  virtual ~View() = default;

  void Draw();

  [[nodiscard]] int top() const { return top_; }
  [[nodiscard]] int bottom() const { return bottom_; }
  [[nodiscard]] int left() const { return left_; }
  [[nodiscard]] int right() const { return right_; }
  [[nodiscard]] double alpha() const { return alpha_; }
  void top(int value) { top_ = value; }
  void bottom(int value) { bottom_ = value; }
  void left(int value) { left_ = value; }
  void right(int value) { right_ = value; }

  void alpha(double value) {
    if (value < 0) {
      value = 0;
    } else if (value > 1) {
      value = 1;
    }
    alpha_ = value;
  }

 protected:
  SdlRenderer& renderer() { return renderer_; }

  virtual void OnPreDraw() {}
  virtual void OnDraw() {}

 private:
  SdlRenderer& renderer_;

  int top_{};
  int bottom_{};
  int left_{};
  int right_{};

  double alpha_{};
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_VIEW_HPP_

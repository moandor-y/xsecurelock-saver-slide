#ifndef XSECURELOCK_SAVER_SLIDE_VIEW_HPP_
#define XSECURELOCK_SAVER_SLIDE_VIEW_HPP_

namespace xsecurelock_saver_slide {
class SdlRenderer;

class View {
 public:
  View() = default;
  View(const View&) = delete;
  View& operator=(const View&) = delete;
  virtual ~View() = default;

  virtual void OnPreDraw(SdlRenderer&) {}
  virtual void Draw(SdlRenderer&) = 0;

  [[nodiscard]] int top() const { return top_; }
  [[nodiscard]] int bottom() const { return bottom_; }
  [[nodiscard]] int left() const { return left_; }
  [[nodiscard]] int right() const { return right_; }
  void top(int value) { top_ = value; }
  void bottom(int value) { bottom_ = value; }
  void left(int value) { left_ = value; }
  void right(int value) { right_ = value; }

  [[nodiscard]] virtual double GetAlpha() const = 0;
  virtual void SetAlpha(double value) = 0;

 private:
  int top_{};
  int bottom_{};
  int left_{};
  int right_{};
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_VIEW_HPP_

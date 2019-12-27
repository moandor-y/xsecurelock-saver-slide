#ifndef XSECURELOCK_SAVER_SLIDE_UTILITY_HPP_
#define XSECURELOCK_SAVER_SLIDE_UTILITY_HPP_

#include <cmath>

namespace xsecurelock_saver_slide {
template <typename T>
T Lerp(const T& a, const T& b, double t) {
  return a * (1 - t) + b * t;
}

template <typename T>
T InterpEaseIn(const T& a, const T& b, double t, double exp) {
  return Lerp(a, b, std::pow(t, exp));
}

template <typename T>
T InterpEaseOut(const T& a, const T& b, double t, double exp) {
  return Lerp(a, b, 1 - std::pow(1 - t, exp));
}

template <typename T>
T InterpEaseInOut(const T& a, const T& b, double t, double exp) {
  return Lerp(a, b,
              t < 0.5
                  ? InterpEaseIn<double>(0, 1, 2 * t, exp) / 2
                  : InterpEaseOut<double>(0, 1, 2 * (t - 0.5), exp) / 2 + 0.5);
}
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_UTILITY_HPP_

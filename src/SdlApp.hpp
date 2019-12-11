#ifndef XSECURELOCK_SAVER_SLIDE_SDL_APP_HPP_
#define XSECURELOCK_SAVER_SLIDE_SDL_APP_HPP_

#include <cstdint>
#include <string>

namespace xsecurelock_saver_slide {
class SdlApp {
 public:
  SdlApp();
  ~SdlApp();
  SdlApp(const SdlApp&) = delete;
  SdlApp& operator=(const SdlApp&) = delete;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_SDL_APP_HPP_

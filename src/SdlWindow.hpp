#ifndef XSECURELOCK_SAVER_SLIDE_SDL_WINDOW_HPP_
#define XSECURELOCK_SAVER_SLIDE_SDL_WINDOW_HPP_

#include <SDL2/SDL.h>

#include <cstdint>
#include <memory>
#include <string>

namespace xsecurelock_saver_slide {
class SdlWindow {
 public:
  SdlWindow(const std::string& title, int x, int y, int w, int h,
            std::uint32_t flags);
  explicit SdlWindow(const void* data);

  SDL_Window* get() { return window_.get(); }

 private:
  struct Deleter {
    void operator()(SDL_Window*) const noexcept;
  };

  std::unique_ptr<SDL_Window, Deleter> window_{};
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_SDL_WINDOW_HPP_

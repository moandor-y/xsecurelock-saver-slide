#ifndef XSECURELOCK_SAVER_SLIDE_SDLSURFACE_HPP_
#define XSECURELOCK_SAVER_SLIDE_SDLSURFACE_HPP_

#include <SDL2/SDL.h>

#include <cstdint>
#include <memory>
#include <string>

namespace xsecurelock_saver_slide {
class SdlTtfFont;
class SdlRwOps;

class SdlSurface {
 public:
  SdlSurface() noexcept = default;
  SdlSurface(SdlTtfFont&, const std::string& text, SDL_Color fg);
  SdlSurface(void* pixels, int width, int height, int depth, int pitch,
             std::uint32_t format);
  SdlSurface(int width, int height, int depth, std::uint32_t format);
  explicit SdlSurface(SdlRwOps&);
  explicit SdlSurface(SdlRwOps&&);

  SdlSurface ConvertSurfaceFormat(std::uint32_t format);
  void Lock();
  void Unlock() noexcept;
  [[nodiscard]] bool MustLock() const noexcept;
  [[nodiscard]] void* pixels() noexcept { return surface_->pixels; }
  [[nodiscard]] int width() const noexcept { return surface_->w; }
  [[nodiscard]] int height() const noexcept { return surface_->h; }
  [[nodiscard]] int pitch() const noexcept { return surface_->pitch; }

  [[nodiscard]] const SDL_PixelFormat& format() const noexcept {
    return *surface_->format;
  }

  SDL_Surface* get() { return surface_.get(); }

 private:
  struct Deleter {
    void operator()(SDL_Surface*) const noexcept;
  };

  std::unique_ptr<SDL_Surface, Deleter> surface_;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_SDLSURFACE_HPP_

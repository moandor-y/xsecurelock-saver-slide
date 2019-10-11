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
  explicit SdlSurface(SdlRwOps&);

  SdlSurface ConvertSurfaceFormat(std::uint32_t format);

  SDL_Surface* get() { return surface_.get(); }

 private:
  struct Deleter {
    void operator()(SDL_Surface*) const noexcept;
  };

  std::unique_ptr<SDL_Surface, Deleter> surface_;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_SDLSURFACE_HPP_

#ifndef XSECURELOCK_SAVER_SLIDE_SDLTTFFONT_HPP_
#define XSECURELOCK_SAVER_SLIDE_SDLTTFFONT_HPP_

#include <SDL2/SDL_ttf.h>

#include <memory>
#include <string>

namespace xsecurelock_saver_slide {
class SdlTtfFont {
 public:
  SdlTtfFont(const std::string& file, int size);

  TTF_Font* get() { return font_.get(); }

 private:
  struct Deleter {
    void operator()(TTF_Font*) const noexcept;
  };

  std::unique_ptr<TTF_Font, Deleter> font_{};
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_SDLTTFFONT_HPP_

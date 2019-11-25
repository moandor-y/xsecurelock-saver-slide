#ifndef XSECURELOCK_SAVER_SLIDE_SDLRENDERER_HPP_
#define XSECURELOCK_SAVER_SLIDE_SDLRENDERER_HPP_

#include <SDL2/SDL.h>

#include <cstdint>
#include <memory>

namespace xsecurelock_saver_slide {
class SdlWindow;
class SdlTexture;

class SdlRenderer {
 public:
  SdlRenderer() noexcept = default;
  SdlRenderer(SdlWindow&, int index, std::uint32_t flags);

  void SetRendererDrawColor(std::uint8_t r, std::uint8_t g, std::uint8_t b,
                            std::uint8_t a);
  void RenderClear();
  void RenderCopy(SdlTexture&, const SDL_Rect* srcrect,
                  const SDL_Rect* dstrect);
  void RenderPresent();
  SDL_RendererInfo GetRendererInfo();

  SDL_Renderer* get() { return renderer_.get(); }

  struct OutputSize {
    int width;
    int height;
  };

  OutputSize GetRendererOutputSize();

 private:
  struct Deleter {
    void operator()(SDL_Renderer*) const noexcept;
  };

  std::unique_ptr<SDL_Renderer, Deleter> renderer_{};
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_SDLRENDERER_HPP_

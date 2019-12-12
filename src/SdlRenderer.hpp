#ifndef XSECURELOCK_SAVER_SLIDE_SDL_RENDERER_HPP_
#define XSECURELOCK_SAVER_SLIDE_SDL_RENDERER_HPP_

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

  void RenderClear();
  void RenderCopy(SdlTexture&, const SDL_Rect* srcrect,
                  const SDL_Rect* dstrect);
  void RenderPresent();
  [[nodiscard]] SDL_RendererInfo GetRendererInfo() const;
  [[nodiscard]] SDL_Texture* GetRenderTarget() const noexcept;
  void SetRenderTarget(SDL_Texture*);

  SDL_Renderer* get() { return renderer_.get(); }

  struct OutputSize {
    int width;
    int height;
  };

  struct Color {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;
  };

  OutputSize GetRendererOutputSize();
  void SetRenderDrawColor(Color);
  [[nodiscard]] Color GetRenderDrawColor() const;

 private:
  struct Deleter {
    void operator()(SDL_Renderer*) const noexcept;
  };

  std::unique_ptr<SDL_Renderer, Deleter> renderer_{};
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_SDL_RENDERER_HPP_

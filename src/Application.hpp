#ifndef XSECURELOCK_SAVER_SLIDE_APPLICATION_HPP_
#define XSECURELOCK_SAVER_SLIDE_APPLICATION_HPP_

#include <future>
#include <queue>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "SdlApp.hpp"
#include "SdlRenderer.hpp"
#include "SdlSurface.hpp"
#include "SdlTexture.hpp"
#include "SdlTtf.hpp"
#include "SdlWindow.hpp"

namespace xsecurelock_saver_slide {
class Application {
 public:
  Application();
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;
  void Run();

 private:
  enum class State { IDLE, FADE_OUT, FADE_IN };

  SdlApp sdl_app_{};
  SdlTtf sdl_ttf_{};
  SdlWindow sdl_window_;
  SdlRenderer sdl_renderer_;

  std::vector<std::string> image_paths_{};
  std::queue<std::string> image_queue_{};
  std::random_device random_{};

  State state_ = State::IDLE;
  SdlTexture foreground_{};
  SdlTexture background_{};
  std::future<std::pair<SdlSurface, SdlSurface>> next_image_{};
  double state_time_remaining_;

  const std::string& NextImagePath();
  SdlTexture TextureFromSurface(SdlSurface&);
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_APPLICATION_HPP_

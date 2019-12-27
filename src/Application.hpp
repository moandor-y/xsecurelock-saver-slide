#ifndef XSECURELOCK_SAVER_SLIDE_APPLICATION_HPP_
#define XSECURELOCK_SAVER_SLIDE_APPLICATION_HPP_

#include <SDL2/SDL.h>

#include <cstdint>
#include <future>
#include <memory>
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
#include "View.hpp"

namespace xsecurelock_saver_slide {
class ImageView;

class Application {
 public:
  Application();
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;
  void Run();

  static std::uint32_t GetPixelFormat();

 private:
  enum class State { kIdle, kTransition };
  enum class TransitionType { kNone, kNoMove, kHMove, kVMove };

  SdlApp sdl_app_{};
  SdlTtf sdl_ttf_{};
  SdlWindow sdl_window_;
  SdlRenderer sdl_renderer_;

  std::vector<std::string> image_paths_{};
  std::queue<std::string> image_queue_{};
  std::random_device random_{};

  State state_ = State::kIdle;

  TransitionType transition_type_ = TransitionType::kNone;
  int transition_margin_start_{};
  int transition_margin_end_{};

  std::unique_ptr<View> content_;
  ImageView* foreground_{};
  ImageView* background_{};

  std::unique_ptr<View> content_next_;
  ImageView* foreground_next_{};
  ImageView* background_next_{};

  std::future<std::pair<SdlSurface, SdlSurface>> next_image_{};
  double state_time_remaining_;

  const std::string& NextImagePath();
  SdlTexture TextureFromSurface(SdlSurface&);
  double GetIdleTime();
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_APPLICATION_HPP_

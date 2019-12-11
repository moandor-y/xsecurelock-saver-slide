#ifndef XSECURELOCK_SAVER_SLIDE_SDL_RW_OPS_HPP_
#define XSECURELOCK_SAVER_SLIDE_SDL_RW_OPS_HPP_

#include <SDL2/SDL.h>

#include <memory>
#include <string>

namespace xsecurelock_saver_slide {
class SdlRwOps {
 public:
  SdlRwOps(const void* mem, int size);
  SdlRwOps(const std::string& file, const std::string& mode);

  SDL_RWops* get() { return rw_.get(); }

 private:
  struct Deleter {
    void operator()(SDL_RWops*) const noexcept;
  };

  std::unique_ptr<SDL_RWops, Deleter> rw_{};
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_SDL_RW_OPS_HPP_

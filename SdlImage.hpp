#ifndef XSECURELOCK_SAVER_SLIDE_SDLIMAGE_HPP_
#define XSECURELOCK_SAVER_SLIDE_SDLIMAGE_HPP_

namespace xsecurelock_saver_slide {
class SdlImage {
 public:
  SdlImage();
  ~SdlImage();
  SdlImage(const SdlImage&) = delete;
  SdlImage& operator=(const SdlImage&) = delete;
};
}  // namespace xsecurelock_saver_slide

#endif  // XSECURELOCK_SAVER_SLIDE_SDLIMAGE_HPP_

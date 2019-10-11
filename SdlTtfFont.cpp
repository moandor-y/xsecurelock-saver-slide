#include "SdlTtfFont.hpp"

#include <SDL2/SDL_ttf.h>

#include <stdexcept>
#include <string>

namespace xsecurelock_saver_slide {
namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

SdlTtfFont::SdlTtfFont(const std::string& file, int size)
    : font_{TTF_OpenFont(file.c_str(), size)} {
  if (font_ == nullptr) {
    throw runtime_error{"Error calling TTF_OpenFont: "s + TTF_GetError()};
  }
}

void SdlTtfFont::Deleter::operator()(TTF_Font* font) const noexcept {
  TTF_CloseFont(font);
}
}  // namespace xsecurelock_saver_slide

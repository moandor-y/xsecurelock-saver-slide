#include "Application.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <exception>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include "SdlRenderer.hpp"
#include "SdlRwOps.hpp"
#include "SdlSurface.hpp"
#include "SdlTexture.hpp"
#include "SdlTtfFont.hpp"
#include "SdlWindow.hpp"

namespace xsecurelock_saver_slide {
namespace {
using std::async;
using std::cout;
using std::endl;
using std::exception;
using std::FILE;
using std::fread;
using std::future;
using std::future_status;
using std::getline;
using std::invalid_argument;
using std::launch;
using std::lround;
using std::move;
using std::out_of_range;
using std::put_time;
using std::runtime_error;
using std::shuffle;
using std::string;
using std::stringstream;
using std::uint64_t;
using std::uniform_int_distribution;
using std::unique_ptr;
using std::vector;

using Size = std::int_fast64_t;

constexpr double kIdleTime = 3;
constexpr double kFadeOutTime = 0.5;
constexpr double kFadeInTime = 0.5;

future<SdlSurface> LoadImage(const string& path) {
  return async(
      launch::async,
      [](string path) {
        SdlRwOps ops{path, "rb"};
        SdlSurface surface{ops};
        return surface.ConvertSurfaceFormat(SDL_PIXELFORMAT_ARGB8888);
      },
      path);
}

SdlWindow CreateWindow() {
  class ConvertException : public exception {};

  try {
    const char* env = getenv("XSCREENSAVER_WINDOW");
    if (env == nullptr) {
      throw ConvertException{};
    }

    uint64_t window;
    try {
      window = std::stoull(env);
    } catch (const invalid_argument&) {
      throw ConvertException{};
    } catch (const out_of_range&) {
      throw ConvertException{};
    }

    return SdlWindow(reinterpret_cast<const void*>(window));

  } catch (const ConvertException&) {
    return SdlWindow{
        "T",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000,
        1000, SDL_WINDOW_SHOWN};
  }
}

stringstream ExecuteCommand(const string& command) {
  vector<char> buffer(0x1000);
  auto close = [](FILE* file) { pclose(file); };
  unique_ptr<FILE, decltype(close)> pipe{popen(command.c_str(), "r"), close};
  if (pipe == nullptr) {
    throw runtime_error{"Failed to call popen"};
  }
  stringstream stream;
  Size size;
  while ((size = fread(buffer.data(), sizeof(buffer[0]), buffer.size(),
                       pipe.get())) > 0) {
    stream.write(buffer.data(), size);
  }
  return stream;
}

SdlTtfFont CreateFont(int size) {
  const char* cmd = getenv("XSECURELOCK_SAVER_SLIDE_FONT_COMMAND");
  if (cmd == nullptr) {
    cmd = "find /usr/share/fonts -name 'NotoSans-Regular.ttf'";
  }
  cout << "Command to find font: " << cmd << endl;

  stringstream stream = ExecuteCommand(cmd);
  string path;
  getline(stream, path);
  return SdlTtfFont{path, size};
}
}  // namespace

void Application::Run() {
  SDL_RendererInfo renderer_info = sdl_renderer_.GetRendererInfo();
  cout << "Renderer texture formats: " << endl;
  for (int i = 0; i < renderer_info.num_texture_formats; ++i) {
    cout << SDL_GetPixelFormatName(renderer_info.texture_formats[i]) << endl;
  }

  SdlTtfFont font_large{CreateFont(192)};
  SdlTtfFont font_small{CreateFont(64)};

  auto time = std::chrono::high_resolution_clock::now();
  bool running = true;
  while (running) {
    auto now = std::chrono::high_resolution_clock::now();
    double time_delta =
        std::chrono::duration_cast<std::chrono::duration<double>>(now - time)
            .count();
    time = now;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    sdl_renderer_.SetRendererDrawColor(0, 0, 0, 0xff);
    sdl_renderer_.RenderClear();

    state_time_remaining_ -= time_delta;
    switch (state_) {
      case State::IDLE: {
        namespace chrono = std::chrono;
        if (state_time_remaining_ < 0 &&
            next_image_.wait_for(chrono::seconds(0)) == future_status::ready) {
          state_time_remaining_ += kFadeOutTime;
          state_ = State::FADE_OUT;
        }
        break;
      }
      case State::FADE_OUT: {
        if (state_time_remaining_ < 0) {
          SdlSurface surface = next_image_.get();
          current_texture_ = TextureFromSurface(surface);
          next_image_ = LoadImage(NextImagePath());

          state_time_remaining_ += kFadeInTime;
          state_ = State::FADE_IN;
        }
        break;
      }
      case State::FADE_IN: {
        if (state_time_remaining_ < 0) {
          state_time_remaining_ += kIdleTime;
          state_ = State::IDLE;
        }
        break;
      }
    }

    double alpha;
    switch (state_) {
      case State::IDLE:
        alpha = 1;
        break;
      case State::FADE_OUT:
        alpha = state_time_remaining_ / kFadeOutTime;
        break;
      case State::FADE_IN:
        alpha = 1 - state_time_remaining_ / kFadeInTime;
        break;
    }
    if (alpha < 0) {
      alpha = 0;
    } else if (alpha > 1) {
      alpha = 1;
    }

    auto screen_size = sdl_renderer_.GetRendererOutputSize();

    {
      SDL_Color text_color{0xff, 0xff, 0xff, 0xff};

      std::time_t t = std::time(nullptr);
      std::tm tm = *std::localtime(&t);
      stringstream stream;
      stream << put_time(&tm, "%A, %B %d");
      SdlSurface surface{font_small, stream.str(), text_color};
      SdlTexture texture{TextureFromSurface(surface)};
      auto texture_attr = texture.QueryTexture();
      SDL_Rect dest{};
      dest.x = 100;
      dest.y = screen_size.height - 100 - texture_attr.height;
      dest.w = texture_attr.width;
      dest.h = texture_attr.height;
      sdl_renderer_.RenderCopy(texture, nullptr, &dest);

      stream = stringstream{};
      stream << put_time(&tm, "%R");
      surface = SdlSurface{font_large, stream.str(), text_color};
      texture = TextureFromSurface(surface);
      texture_attr = texture.QueryTexture();
      dest.y -= texture_attr.height;
      dest.w = texture_attr.width;
      dest.h = texture_attr.height;
      sdl_renderer_.RenderCopy(texture, nullptr, &dest);
    }

    {
      auto texture_attr = current_texture_.QueryTexture();
      double texture_ratio =
          static_cast<double>(texture_attr.width) / texture_attr.height;
      double screen_ratio =
          static_cast<double>(screen_size.width) / screen_size.height;
      SDL_Rect dest{};
      if (texture_ratio > screen_ratio) {
        double width = texture_ratio * screen_size.height;
        dest.x = lround((screen_size.width - width) / 2);
        dest.y = 0;
        dest.w = lround(width);
        dest.h = screen_size.height;
      } else {
        double height = screen_size.width / texture_ratio;
        dest.x = 0;
        dest.y = lround((screen_size.height - height) / 2);
        dest.w = screen_size.width;
        dest.h = lround(height);
      }
      current_texture_.SetTextureAlphaMod(lround(alpha * 255));
      sdl_renderer_.RenderCopy(current_texture_, nullptr, &dest);
    }

    sdl_renderer_.RenderPresent();
  }
}

Application::Application()
    : sdl_window_{CreateWindow()},
      sdl_renderer_{sdl_window_, -1, SDL_RENDERER_ACCELERATED},
      state_time_remaining_{kIdleTime} {
  {
    const char* cmd = getenv("XSECURELOCK_SAVER_SLIDE_LIST_IMAGES_COMMAND");
    if (cmd == nullptr) {
      cmd = "find ~/Pictures -type f -name '*.png'";
    }
    cout << "Command to list images: " << cmd << endl;

    stringstream stream = ExecuteCommand(cmd);
    string path;
    while (getline(stream, path)) {
      image_paths_.push_back(move(path));
    }
  }
  cout << "Found " << image_paths_.size() << " images: " << endl;
  for (const string& path : image_paths_) {
    cout << path << endl;
  }
  if (image_paths_.size() < 2) {
    throw runtime_error{"Too few images to load"};
  }

  Size queue_size = (image_paths_.size() + 1) / 2;
  shuffle(image_paths_.begin(), image_paths_.end(), random_);
  for (Size i = 0; i < queue_size; ++i) {
    image_queue_.push(move(image_paths_.back()));
    image_paths_.pop_back();
  }

  SdlSurface surface = LoadImage(NextImagePath()).get();
  current_texture_ = TextureFromSurface(surface);
  next_image_ = LoadImage(NextImagePath());
}

const string& Application::NextImagePath() {
  image_paths_.push_back(move(image_queue_.front()));
  image_queue_.pop();
  Size pos = uniform_int_distribution<>{
      0, static_cast<int>(image_paths_.size() - 1)}(random_);
  image_queue_.push(move(image_paths_[pos]));
  image_paths_.erase(image_paths_.begin() + pos);
  return image_queue_.front();
}

SdlTexture Application::TextureFromSurface(SdlSurface& surface) {
  SdlTexture texture{sdl_renderer_, surface};
  texture.SetTextureBlendMode(SDL_BLENDMODE_BLEND);
  return texture;
}
}  // namespace xsecurelock_saver_slide

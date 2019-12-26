#include "Application.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <exception>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <utility>
#include <vector>

#include "ImageView.hpp"
#include "SdlRwOps.hpp"
#include "SdlSurface.hpp"
#include "SdlTexture.hpp"
#include "SdlTtfFont.hpp"
#include "SdlWindow.hpp"
#include "ViewGroup.hpp"

namespace xsecurelock_saver_slide {
namespace {
using std::async;
using std::clog;
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
using std::make_unique;
using std::move;
using std::out_of_range;
using std::pair;
using std::put_time;
using std::runtime_error;
using std::shuffle;
using std::string;
using std::stringstream;
using std::uint32_t;
using std::uint64_t;
using std::uniform_int_distribution;
using std::unique_ptr;
using std::vector;

using Size = std::int_fast64_t;

constexpr double kIdleTime = 15;
constexpr double kFadeOutTime = 0.5;
constexpr double kFadeInTime = 0.5;

constexpr uint32_t kPixelFormat = SDL_PIXELFORMAT_ARGB8888;

future<pair<SdlSurface, SdlSurface>> LoadImage(const string& path,
                                               int screen_width,
                                               int screen_height) {
  auto load = [screen_width, screen_height](
                  const string& path) -> pair<SdlSurface, SdlSurface> {
    pair<SdlSurface, SdlSurface> result;
    cv::Mat image = cv::imread(path);
    int image_width = image.cols;
    int image_height = image.rows;
    const double image_ratio = static_cast<double>(image_width) / image_height;
    const double screen_ratio =
        static_cast<double>(screen_width) / screen_height;

    if (image_ratio > screen_ratio) {
      image_width = screen_width;
      image_height = static_cast<int>(lround(image_width / image_ratio));
    } else {
      image_height = screen_height;
      image_width = static_cast<int>(lround(image_height * image_ratio));
    }
    cv::resize(image, image, {image_width, image_height}, 0, 0, CV_INTER_AREA);

    vector<unsigned char> buffer;
    cv::imencode(".bmp", image, buffer);
    result.first =
        SdlSurface{SdlRwOps{buffer.data(), static_cast<int>(buffer.size())}}
            .ConvertSurfaceFormat(kPixelFormat);

    image = cv::imread(path);
    image_width = image.cols;
    image_height = image.rows;

    cv::Rect rect;
    if (image_ratio > screen_ratio) {
      int width = static_cast<int>(lround(image_height * screen_ratio));
      rect = {(image_width - width) / 2, 0, width, image_height};
    } else {
      int height = static_cast<int>(lround(image_width / screen_ratio));
      rect = {0, (image_height - height) / 2, image_width, height};
    }
    image = image(rect);
    cv::resize(image, image, {screen_width, screen_height}, 0, 0,
               CV_INTER_LANCZOS4);
    cv::GaussianBlur(image, image, {}, screen_width * 0.006);

    buffer.clear();
    cv::imencode(".bmp", image, buffer);
    result.second =
        SdlSurface{SdlRwOps{buffer.data(), static_cast<int>(buffer.size())}}
            .ConvertSurfaceFormat(kPixelFormat);

    return result;
  };

  return async(launch::async, load, path);
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
        "T",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 2560,
        1440, SDL_WINDOW_SHOWN};
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
  clog << "Command to find font: " << cmd << endl;

  stringstream stream = ExecuteCommand(cmd);
  string path;
  getline(stream, path);
  return SdlTtfFont{path, size};
}
}  // namespace

void Application::Run() {
  SDL_RendererInfo renderer_info = sdl_renderer_.GetRendererInfo();
  clog << "Renderer texture formats: " << endl;
  for (Size i = 0; i < renderer_info.num_texture_formats; ++i) {
    clog << SDL_GetPixelFormatName(renderer_info.texture_formats[i]) << endl;
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

    auto screen_size = sdl_renderer_.GetRendererOutputSize();

    sdl_renderer_.SetRenderDrawColor({0, 0, 0, 0xff});
    sdl_renderer_.RenderClear();

    state_time_remaining_ -= time_delta;
    switch (state_) {
      case State::IDLE: {
        namespace chrono = std::chrono;
        if (state_time_remaining_ < 0 &&
            next_image_.wait_for(chrono::seconds(0)) == future_status::ready) {
          state_time_remaining_ = kFadeOutTime;
          state_ = State::FADE_OUT;
        }
        break;
      }
      case State::FADE_OUT: {
        if (state_time_remaining_ < 0) {
          pair<SdlSurface, SdlSurface> surfaces = next_image_.get();
          foreground_->SetImage(surfaces.first);
          background_->SetImage(surfaces.second);
          next_image_ =
              LoadImage(NextImagePath(), screen_size.width, screen_size.height);

          state_time_remaining_ = kFadeInTime;
          state_ = State::FADE_IN;
        }
        break;
      }
      case State::FADE_IN: {
        if (state_time_remaining_ < 0) {
          state_time_remaining_ = kIdleTime;
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

    content_->top(0);
    content_->left(0);
    content_->bottom(screen_size.height);
    content_->right(screen_size.width);

    background_->top(0);
    background_->left(0);
    background_->bottom(screen_size.height);
    background_->right(screen_size.width);

    {
      int image_width = foreground_->surface_width();
      int image_height = foreground_->surface_height();
      double image_ratio = static_cast<double>(image_width) / image_height;
      double screen_ratio =
          static_cast<double>(screen_size.width) / screen_size.height;

      if (image_ratio > screen_ratio) {
        foreground_->left(0);
        foreground_->top((screen_size.height - image_height) / 2);
      } else {
        foreground_->left(screen_size.width - image_width);
        foreground_->top(0);
      }
      foreground_->right(foreground_->left() + image_width);
      foreground_->bottom(foreground_->top() + image_height);
    }

    content_->alpha(alpha);
    content_->Draw();

    sdl_renderer_.RenderPresent();
  }
}

Application::Application()
    : sdl_window_{CreateWindow()},
      sdl_renderer_{
          sdl_window_,
          -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC,
      },
      state_time_remaining_{kIdleTime} {
  {
    const char* cmd = getenv("XSECURELOCK_SAVER_SLIDE_LIST_IMAGES_COMMAND");
    if (cmd == nullptr) {
      cmd = "find ~/Pictures -type f -name '*.png'";
    }
    clog << "Command to list images: " << cmd << endl;

    stringstream stream = ExecuteCommand(cmd);
    string path;
    while (getline(stream, path)) {
      image_paths_.push_back(move(path));
    }
  }
  clog << "Found " << image_paths_.size() << " images: " << endl;
  for (const string& path : image_paths_) {
    clog << path << endl;
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

  auto screen_size = sdl_renderer_.GetRendererOutputSize();

  pair<SdlSurface, SdlSurface> surfaces =
      LoadImage(NextImagePath(), screen_size.width, screen_size.height).get();
  {
    unique_ptr<ViewGroup> content = make_unique<ViewGroup>(sdl_renderer_);
    unique_ptr<ImageView> foreground =
        make_unique<ImageView>(sdl_renderer_, surfaces.first);
    unique_ptr<ImageView> background =
        make_unique<ImageView>(sdl_renderer_, surfaces.second);
    foreground_ = foreground.get();
    background_ = background.get();
    content->AddChild(move(background));
    content->AddChild(move(foreground));
    content_ = move(content);
  }
  next_image_ =
      LoadImage(NextImagePath(), screen_size.width, screen_size.height);
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

uint32_t Application::GetPixelFormat() { return kPixelFormat; }
}  // namespace xsecurelock_saver_slide

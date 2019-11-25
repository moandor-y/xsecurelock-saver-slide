# XSecureLock Saver Slide

A screen saver module for [XSecureLock](https://github.com/google/xsecurelock)
which shows pictures as a slide show.

## Building

### Requirements

- [CMake](https://cmake.org/) 3.13 or later
- [SDL](https://www.libsdl.org/) 2.0
- [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/) 2.0
- [OpenCV](https://opencv.org/)

### Steps

```
git clone https://github.com/Moandor-y/xsecurelock-saver-slide.git
cd xsecurelock-saver-slide
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
Then expect the executable to be created at `./xsecurelock_saver_slide`.

## Using

1.  Install the executable together with the included `auth_` modules (default
    location: `/usr/local/libexec/xsecurelock/helpers`) and name it
    `saver_slide`. (See [XSecureLock - Writing Your Own Module](
    https://github.com/google/xsecurelock#writing-your-own-module-2))
2.  Pass `XSECURELOCK_SAVER=saver_slide` by environment variables.
3.  (Optional) Pass
    `XSECURELOCK_SAVER_SLIDE_LIST_IMAGES_COMMAND=<your command>` if you want
    to provide your own command to list all images to be shown. Defaults to
    `find ~/Pictures -type f -name '*.png'`. **There must be at least 2 images
    found by this command, otherwise the program will crash.**
4.  (Optional) Pass `XSECURELOCK_SAVER_SLIDE_FONT_COMMAND=<your command>` if
    you want to provide your own command to find the font used to display date
    and time. Default to `find /usr/share/fonts -name 'NotoSans-Regular.ttf'`.
    **A TTF font must be found by this command, otherwise the program will
    crash.**

See the [installation](https://github.com/google/xsecurelock#installation) and
[options](https://github.com/google/xsecurelock#options) of XSecureLock for
more information.

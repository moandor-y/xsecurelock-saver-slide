cmake_minimum_required(VERSION 3.13)
project(xsecurelock_saver_slide)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)

add_executable(xsecurelock_saver_slide main.cpp Application.cpp Application.hpp SdlApp.cpp SdlApp.hpp SdlTtf.cpp SdlTtf.hpp SdlWindow.cpp SdlWindow.hpp SdlRenderer.cpp SdlRenderer.hpp SdlTexture.cpp SdlTexture.hpp SdlTtfFont.cpp SdlTtfFont.hpp SdlSurface.cpp SdlSurface.hpp SdlRwOps.cpp SdlRwOps.hpp)

FIND_LIBRARY(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        HINTS
        ${SDL2}
        $ENV{SDL2}
        $ENV{SDL2_TTF}
        PATH_SUFFIXES lib64 lib
        lib/x64
        x86_64-w64-mingw32/lib
        PATHS
        /sw
        /opt/local
        /opt/csw
        /opt
        )


target_link_libraries(xsecurelock_saver_slide ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARY} ${OpenCV_LIBS} pthread)

cmake_minimum_required(VERSION 3.13)
project(xsecurelock_saver_slide)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)

add_executable(
        xsecurelock_saver_slide
        src/main.cpp
        src/Application.cpp
        src/Application.hpp
        src/SdlApp.cpp
        src/SdlApp.hpp
        src/SdlTtf.cpp
        src/SdlTtf.hpp
        src/SdlWindow.cpp
        src/SdlWindow.hpp
        src/SdlRenderer.cpp
        src/SdlRenderer.hpp
        src/SdlTexture.cpp
        src/SdlTexture.hpp
        src/SdlTtfFont.cpp
        src/SdlTtfFont.hpp
        src/SdlSurface.cpp
        src/SdlSurface.hpp
        src/SdlRwOps.cpp
        src/SdlRwOps.hpp
        src/View.hpp
        src/ImageView.hpp
        src/ImageView.cpp
        src/ViewGroup.hpp
        src/ViewGroup.cpp
        src/TextureView.hpp
        src/TextureView.cpp
        src/View.cpp
        src/Utility.hpp
)

FIND_LIBRARY(
        SDL2_TTF_LIBRARY
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


target_link_libraries(
        xsecurelock_saver_slide
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARY}
        ${OpenCV_LIBS}
        pthread
)

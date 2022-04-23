#**************************************************************************************************
#
#   raylib makefile for Desktop platforms, Raspberry Pi, Android and HTML5
#
#   Copyright (c) 2013-2021 Ramon Santamaria (@raysan5)
#
#   This software is provided "as-is", without any express or implied warranty. In no event
#   will the authors be held liable for any damages arising from the use of this software.
#
#   Permission is granted to anyone to use this software for any purpose, including commercial
#   applications, and to alter it and redistribute it freely, subject to the following restrictions:
#
#     1. The origin of this software must not be misrepresented; you must not claim that you
#     wrote the original software. If you use this software in a product, an acknowledgment
#     in the product documentation would be appreciated but is not required.
#
#     2. Altered source versions must be plainly marked as such, and must not be misrepresented
#     as being the original software.
#
#     3. This notice may not be removed or altered from any source distribution.
#
#**************************************************************************************************

.PHONY: all clean

# Define required variables
PROJECT_NAME       ?= Sokobus
RAYLIB_VERSION     ?= 4.0.0
RAYLIB_PATH        ?= C:/raylib-web

# Define default options

# One of PLATFORM_DESKTOP, PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
PLATFORM           ?= PLATFORM_WEB

# Locations of your newly installed library and associated headers.
# On Linux, if you have installed raylib but cannot compile the examples, check that
# the *_INSTALL_PATH values here are the same as those in src/Makefile or point to known locations.
# To enable system-wide compile-time and runtime linking to libraylib.so, run ../src/$ sudo make install RAYLIB_LIBTYPE_SHARED.
# To enable compile-time linking to a special version of libraylib.so, change these variables here.
# To enable runtime linking to a special version of libraylib.so, see EXAMPLE_RUNTIME_PATH below.
# If there is a libraylib in both EXAMPLE_RUNTIME_PATH and RAYLIB_INSTALL_PATH, at runtime,
# the library at EXAMPLE_RUNTIME_PATH, if present, will take precedence over the one at RAYLIB_INSTALL_PATH.

# Desired full path to libraylib. No relative paths.
DESTDIR               ?= /usr/local
RAYLIB_INSTALL_PATH   ?= $(DESTDIR)/lib

# raylib header and associated source files
RAYLIB_H_INSTALL_PATH ?= $(DESTDIR)/include

# Library type used for raylib: STATIC (.a) or SHARED (.so/.dll)
RAYLIB_LIBTYPE        ?= STATIC

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= RELEASE

# Use external GLFW library instead of rglfw module
# TODO: Review usage on Linux. Target version of choice. Switch on -lglfw or -lglfw3
USE_EXTERNAL_GLFW     ?= FALSE

# Use Wayland display server protocol on Linux desktop
# by default it uses X11 windowing system
USE_WAYLAND_DISPLAY   ?= FALSE

# Emscripten required variables
EMSDK_PATH         ?= C:/emsdk
EMSCRIPTEN_PATH    ?= $(EMSDK_PATH)/upstream/emscripten
CLANG_PATH          = $(EMSDK_PATH)/upstream/bin
PYTHON_PATH         = $(EMSDK_PATH)/python/3.9.2-1_64bit
NODE_PATH           = $(EMSDK_PATH)/node/14.15.5_64bit/bin
export PATH         = $(EMSDK_PATH);$(EMSCRIPTEN_PATH);$(CLANG_PATH);$(NODE_PATH);$(PYTHON_PATH);C:\raylib\MinGW\bin:$$(PATH)

# Define raylib release directory for compiled library.
# RAYLIB_RELEASE_PATH points to provided binaries or your freshly built version
RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src

# EXAMPLE_RUNTIME_PATH embeds a custom runtime location of libraylib.so or other desired libraries
# into each example binary compiled with RAYLIB_LIBTYPE=SHARED. It defaults to RAYLIB_RELEASE_PATH
# so that these examples link at runtime with your version of libraylib.so in ../release/libs/linux
# without formal installation from ../src/Makefile. It aids portability and is useful if you have
# multiple versions of raylib, have raylib installed to a non-standard location, or want to
# bundle libraylib.so with your game. Change it to your liking.
# NOTE: If, at runtime, there is a libraylib.so at both EXAMPLE_RUNTIME_PATH and RAYLIB_INSTALL_PATH,
# The library at EXAMPLE_RUNTIME_PATH, if present, will take precedence over RAYLIB_INSTALL_PATH,
# Implemented for LINUX below with CFLAGS += -Wl,-rpath,$(EXAMPLE_RUNTIME_PATH)
# To see the result, run readelf -d core/core_basic_window; looking at the RPATH or RUNPATH attribute.
# To see which libraries a built example is linking to, ldd core/core_basic_window;
# Look for libraylib.so.1 => $(RAYLIB_INSTALL_PATH)/libraylib.so.1 or similar listing.
EXAMPLE_RUNTIME_PATH   ?= $(RAYLIB_RELEASE_PATH)

CC = emcc

# Define default make program
MAKE    ?= make

# Define compiler flags:
#  -O1                  defines optimization level
#  -g                   include debug information on compilation
#  -s                   strip unnecessary data from build
#  -Wall                turns on most, but not all, compiler warnings
#  -std=c99             defines C language mode (standard C from 1999 revision)
#  -std=gnu99           defines C language mode (GNU C from 1999 revision)
#  -Wno-missing-braces  ignore invalid warning (GCC bug 53119)
#  -D_DEFAULT_SOURCE    use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
CFLAGS += -Wall -pedantic-errors -std=c++20 -D_DEFAULT_SOURCE

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -s ASSERTIONS=1 --profiling
else
    CFLAGS += -Os
endif

# -Os                        # size optimization
# -O2                        # optimization level 2, if used, also set --memory-init-file 0
# -s USE_GLFW=3              # Use glfw3 library (context/input management)
# -s ALLOW_MEMORY_GROWTH=1   # to allow memory resizing -> WARNING: Audio buffers could FAIL!
# -s TOTAL_MEMORY=16777216   # to specify heap memory size (default = 16MB) (67108864 = 64MB)
# -s USE_PTHREADS=1          # multithreading support
# -s WASM=0                  # disable Web Assembly, emitted by default
# -s ASYNCIFY                # lets synchronous C/C++ code interact with asynchronous JS
# -s FORCE_FILESYSTEM=1      # force filesystem to load/save files data
# -s ASSERTIONS=1            # enable runtime checks for common memory allocation errors (-O1 and above turn it off)
# --profiling                # include information for code profiling
# --memory-init-file 0       # to avoid an external memory initialization code file (.mem)
# --preload-file resources   # specify a resources folder for data compilation
# --source-map-base          # allow debugging in browser with source map
CFLAGS += -s USE_GLFW=3 -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --preload-file ../assets

# Define a custom shell .html and output extension
CFLAGS += --shell-file $(RAYLIB_PATH)/src/shell.html
EXT = .html

# Define include paths for required headers
# NOTE: Several external required libraries (stb and others)
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -IC:/emsdk/upstream/emscripten/system/include -IC:/mingw64/include

# Define library paths containing required libs.
LDFLAGS = -L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src

# Libraries for web (HTML5) compiling
LDLIBS = $(RAYLIB_RELEASE_PATH)/libraylib.a

# Define all source files required
PROJECT_SOURCE_FILES ?= \
    Button.cpp \
    Game.cpp \
    GameState.cpp \
    main.cpp \
    MenuState.cpp \
    WinState.cpp \
    Player.cpp \
    State.cpp \
    PauseState.cpp \
    InputState.cpp \
    SettingsState.cpp \
    TextButton.cpp
# EditorState.cpp \

# Define all object files from source files
OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))

MAKEFILE_PARAMS = $(PROJECT_NAME)

# Default target entry
# NOTE: We call this Makefile target or Makefile.Android target
all:
	$(MAKE) $(MAKEFILE_PARAMS)

# Project target defined by PROJECT_NAME
$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

# Compile source files
# NOTE: This pattern will compile every module defined on $(OBJS)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

# Clean everything
clean:
	del *.html *.js *.wasm *.data
	@echo Cleaning done

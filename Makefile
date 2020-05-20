#**************************************************************************************************
#
#   raylib makefile for Desktop platforms, Raspberry Pi, Android and HTML5
#
#   Copyright (c) 2013-2019 Ramon Santamaria (@raysan5)
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


# # # # # # # # # # # # #
#                       #
#     PHONY TARGETS     #
#                       #
# # # # # # # # # # # # #

.PHONY: all clean


# # # # # # # # # # # # #
#                       #
#   PROJECT AND PATHS   #
#                       #
# # # # # # # # # # # # #

# Define required raylib variables
PROJECT_NAME       ?= cellular_automata
RAYLIB_VERSION     ?= 2.5.0
RAYLIB_API_VERSION ?= 251
RAYLIB_PATH        ?= E:/Cpp/raylib/raylib-master
RAYGUI_PATH        ?= E:/Cpp/raylib/raygui-master

# Define compiler path on Windows
COMPILER_PATH      ?= E:/Cpp/mingw-w64/mingw64/bin

# Define default C compiler: gcc
# NOTE: define g++ compiler if using C++
CC = gcc

# Define default make program: Mingw32-make
MAKE = mingw32-make

# Define default options
# One of PLATFORM_DESKTOP, PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
PLATFORM           ?= PLATFORM_DESKTOP

# Define all source files required
PROJECT_SOURCE_FILES ?= \
    main.c \
    screens/screens.c \
    screens/screen_logo.c \
    screens/screen_options.c \
    screens/screen_gameplay.c \
    screens/screen_ending.c \
    screens/screen_title.c

# Define all object files from source files
OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))

# Library type used for raylib: STATIC (.a) or SHARED (.so/.dll)
RAYLIB_LIBTYPE        ?= STATIC

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= RELEASE

# Use external GLFW library instead of rglfw module
# TODO: Review usage on Linux. Target version of choice. Switch on -lglfw or -lglfw3
USE_EXTERNAL_GLFW     ?= FALSE

# Determine PLATFORM_OS in case PLATFORM_DESKTOP selected
PLATFORM_OS=WINDOWS
export PATH := $(COMPILER_PATH):$(PATH)

# Define raylib release directory for compiled library.
# RAYLIB_RELEASE_PATH points to provided binaries or your freshly built version
RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src



# # # # # # # # # # # # #
#                       #
#        CFLAGS         #
#                       #
# # # # # # # # # # # # #

# Define compiler flags:
#  -O1                  defines optimization level
#  -g                   include debug information on compilation
#  -s                   strip unnecessary data from build
#  -Wall                turns on most, but not all, compiler warnings
#  -std=c99             defines C language mode (standard C from 1999 revision)
#  -std=gnu99           defines C language mode (GNU C from 1999 revision)
#  -Wno-missing-braces  ignore invalid warning (GCC bug 53119)
#  -D_DEFAULT_SOURCE    use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
# Additional flags for compiler (if desired)
#CFLAGS += -Wextra -Wmissing-prototypes -Wstrict-prototypes
CFLAGS += -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces
ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g
else
    CFLAGS += -s -O1
endif


# # # # # # # # # # # # #
#                       #
#        INCLUDES       #
#                       #
# # # # # # # # # # # # #

# Define include paths for required headers
# NOTE: Several external required libraries (stb and others)
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -I$(RAYGUI_PATH)/src


# # # # # # # # # # # # #
#                       #
#        LDFLAGS        #
#                       #
# # # # # # # # # # # # #

# Define library paths containing required libs.
LDFLAGS = -L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src

ifeq ($(PLATFORM_OS),WINDOWS)
    # resource file contains windows executable icon and properties
    #####LDFLAGS += $(RAYLIB_PATH)/src/raylib.rc.data 
    # -Wl,--subsystem,windows hides the console window
    ifeq ($(BUILD_MODE), RELEASE)
        LDFLAGS += -Wl,--subsystem,windows
    endif
endif

# Define any libraries required on linking
# if you want to link libraries (libname.so or libname.a), use the -lname
ifeq ($(PLATFORM_OS),WINDOWS)
    # Libraries for Windows desktop compilation
    # NOTE: WinMM library required to set high-res timer resolution
    LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    # Required for physac examples
    LDLIBS += -static -lpthread
endif


# # # # # # # # # # # # #
#                       #
#   MAKEFILE TARGETS    #
#                       #
# # # # # # # # # # # # #

# Default target entry
# NOTE: We call this Makefile target or Makefile.Android target
all:
	$(MAKE) $(PROJECT_NAME)

# Project target defined by PROJECT_NAME
$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

# Compile source files
# NOTE: This pattern will compile every module defined on $(OBJS)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

# Clean everything
clean:
	del *.o *.exe /s


	@echo Cleaning done

re: clean all

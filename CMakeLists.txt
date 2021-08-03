cmake_minimum_required(VERSION 3.19)
project(hackathon-starter LANGUAGES CXX	VERSION 0.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# C++ standard
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# Sets the default build mode on Release
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RELEASE" CACHE STRING "" FORCE)
endif()
# Warnings for Debug mode
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code -Wuninitialized")
# Speed optimization for Release mode
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Os")

# Useful pre-compile definitions
if(CMAKE_BUILD_TYPE MATCHES "Debug")
	add_compile_definitions(DEBUG_MODE)
endif()
if(CMAKE_BUILD_TYPE MATCHES "Release")
	add_compile_definitions(RELEASE_MODE)
endif()
add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")
add_compile_definitions(PROJECT_VERSION="${PROJECT_VERSION}")
add_compile_definitions(GLOBAL_PATH="${CMAKE_SOURCE_DIR}")

# Qt5 import
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

# Adding subdirectories
add_subdirectory(src)
enable_testing()
add_subdirectory(test)
#
# Face Smash
#

cmake_minimum_required(VERSION 3.9)

#
# Building in-tree is not allowed (we take care of your craziness).
#

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
endif()

#
# Project configuration
#

project(facesmash VERSION 0.0.1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(SETTINGS_ORGANIZATION "Michele Caini")
set(SETTINGS_APPLICATION ${PROJECT_NAME})
set(PROJECT_AUTHOR "Michele Caini")
set(PROJECT_AUTHOR_EMAIL "michele.caini@gmail.com")

message("*")
message("* ${PROJECT_NAME} v${PROJECT_VERSION} (${CMAKE_BUILD_TYPE})")
message("* Copyright (c) 2017 ${PROJECT_AUTHOR} <${PROJECT_AUTHOR_EMAIL}>")
message("*")

#
# Compiler stuff
#

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DRELEASE")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DDEBUG")

if(ASAN)
    message("-- Address sanitizer enabled")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif(ASAN)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-mismatched-tags")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-mismatched-tags")
endif()


#
# Dependencies
#

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${facesmash_SOURCE_DIR}/cmake/module)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BLAS REQUIRED)
find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(
    VISAGE_SDK_LIBRARIES
    ${facesmash_SOURCE_DIR}/deps/visageSDK/lib/libVisageVision.a
    ${facesmash_SOURCE_DIR}/deps/visageSDK/lib/libVisageAnalyser.a
)

# this is necessary on archlinux to disable the default Position Independent Executable
link_libraries("-no-pie")
link_libraries("${BLAS_LINKER_FLAGS}")

option(BUILD_TESTING "Disable testing with ctest for EnTT." OFF)
configure_file(${facesmash_SOURCE_DIR}/cmake/in/EnTT.in ${facesmash_SOURCE_DIR}/deps/EnTT/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . WORKING_DIRECTORY ${facesmash_SOURCE_DIR}/deps/EnTT)
execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${facesmash_SOURCE_DIR}/deps/EnTT)
include_directories(
    ${facesmash_SOURCE_DIR}/deps/EnTT/src/src
    ${facesmash_SOURCE_DIR}/deps/visageSDK/include
)

#
# The Game
#

add_subdirectory(src)
add_executable(facesmash ${FACE_SMASH_SOURCES})

target_include_directories(
    facesmash
    PRIVATE ${SDL2_INCLUDE_DIR}
    PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS}
    PRIVATE ${SDL2_MIXER_INCLUDE_DIRS}
    PRIVATE ${SDL2_TTF_INCLUDE_DIRS}
)

target_link_libraries(
    facesmash
    PRIVATE ${SDL2_LIBRARY}
    PRIVATE ${SDL2_IMAGE_LIBRARIES}
    PRIVATE ${SDL2_MIXER_LIBRARIES}
    PRIVATE ${SDL2_TTF_LIBRARIES}
    PRIVATE ${ZLIB_LIBRARIES}
    PRIVATE ${BLAS_LIBRARIES}
    PRIVATE ${VISAGE_SDK_LIBRARIES}
)

add_definitions(-DVISAGE_STATIC)

#
# Package
#

install(
    DIRECTORY
        ${facesmash_SOURCE_DIR}/assets/png
        ${facesmash_SOURCE_DIR}/assets/font
    DESTINATION ${facesmash_BINARY_DIR}/game
)

install(
    TARGETS facesmash
    DESTINATION ${facesmash_BINARY_DIR}/game
)

#
# AOB
#

add_custom_target(
    facesmash_aob
    SOURCES
        AUTHORS
        LICENSE
        README.md
        TODO
)

cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules")

# use macros
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Macros.cmake)

# determine the build type
lug_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

if(ANDROID)
    populate_android_infos()
endif()

# set the path of thirdparty
lug_set_option(LUG_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty" STRING "Choose the path for the thirdparty directory")

# project name
project(triangle)

# use config
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Config.cmake)

# use sample' macros
include(${PROJECT_SOURCE_DIR}/../Macros.cmake)

set(SRC
    src/Application.cpp
    src/main.cpp
)
source_group("src" FILES ${SRC})

set(INC
    include/Application.hpp
)
source_group("inc" FILES ${INC})

set(SHADERS
    gui.frag
    gui.vert
)

set(LUG_RESOURCES
    shaders/forward/shader.frag
    shaders/forward/shader.vert
)

include_directories(include)

lug_add_sample(triangle
               SOURCES ${SRC} ${INC}
               DEPENDS core graphics system window math
               SHADERS ${SHADERS}
               LUG_RESOURCES ${LUG_RESOURCES}
)

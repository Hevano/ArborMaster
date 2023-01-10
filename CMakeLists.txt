cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    ArborMaster
    VERSION 0.1.0
    DESCRIPTION "Visual designer tool for behaviour trees."
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Add third party dependencies ----
add_subdirectory(thirdparty)

#######
#GLFW
#######

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glfw3)

include_directories(ArborMaster ${GLFW_INCLUDE_DIR})


# ---- Declare library ----

add_library(
    ArborMaster_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    ArborMaster_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(ArborMaster_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

# glob is not modern cmake, find better alternative
file (GLOB sources  "./source/executable/*.cpp" "./source/executable/*.h")

add_executable(ArborMaster_exe ${sources})
add_executable(ArborMaster::exe ALIAS ArborMaster_exe)

set_property(TARGET ArborMaster_exe PROPERTY OUTPUT_NAME ArborMaster)

target_compile_features(ArborMaster_exe PRIVATE cxx_std_20)

target_link_libraries(ArborMaster_exe PRIVATE ArborMaster_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT ArborMaster_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of ArborMaster"
  )
endif()

include(cmake/dev-mode.cmake)

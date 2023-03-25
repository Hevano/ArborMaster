cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)
include(CMakePrintHelpers)

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
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty)

find_package(OpenGL REQUIRED)

#######
#GLFW
#######

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glfw3)

include_directories(ArborMaster ${GLFW_INCLUDE_DIR})

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.80.0
)

FetchContent_MakeAvailable(Boost)


# ---- Declare library ----

file (GLOB lib_sources  "./source/lib/*.cpp" "./source/lib/*.h")

add_library(
    ArborMaster_lib OBJECT
    source/lib.hpp
    source/lib.cpp
    ${lib_sources}
)

target_include_directories(
    ArborMaster_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
    ${Boost_INCLUDE_DIRS}
)

target_compile_features(ArborMaster_lib PUBLIC cxx_std_20)

target_link_libraries(ArborMaster_lib PUBLIC glfw OpenGL::GL glad imgui imnodes Boost::interprocess)

target_link_libraries(ArborMaster_lib PUBLIC nlohmann_json::nlohmann_json)




# ---- Declare adapter library ----

file (GLOB adapter_sources  "./source/adapter/*.cpp" "./source/adapter/*.h")

add_library(
    ArborMaster_adapter STATIC
    ${adapter_sources}
)

target_include_directories(
    ArborMaster_adapter ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    ${Boost_INCLUDE_DIRS}
)

target_compile_features(ArborMaster_adapter PUBLIC cxx_std_20)

target_link_libraries(ArborMaster_adapter PUBLIC Boost::interprocess nlohmann_json::nlohmann_json)

# set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "./source/adapter/ArborMasterAdapter.h")


# ---- Declare executable ----

# glob is not modern cmake, find better alternative
file (GLOB exe_sources  "./source/exe/*.cpp" "./source/exe/*.h")

add_executable(ArborMaster_exe source/main.cpp ${exe_sources})
add_executable(ArborMaster::exe ALIAS ArborMaster_exe)

set_property(TARGET ArborMaster_exe PROPERTY OUTPUT_NAME ArborMaster)

target_compile_features(ArborMaster_exe PRIVATE cxx_std_20)

target_link_libraries(ArborMaster_exe PRIVATE ArborMaster_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

include(GNUInstallDirs)

message(STATUS ${CMAKE_INSTALL_LIBDIR})

install(TARGETS ArborMaster_adapter boost_interprocess nlohmann_json
        EXPORT ArborMaster_adapterTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

set(adapterHeaders
    source/adapter/Adapter.h
    source/adapter/Debugger.h
)

install(FILES ${adapterHeaders} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})





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



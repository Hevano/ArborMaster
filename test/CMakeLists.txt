cmake_minimum_required(VERSION 3.14)
# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(ArborMasterTests LANGUAGES CXX)

# ---- Tests ----
set(CMAKE_CXX_STANDARD 14)

set(cxx_strict_flags "/W0")

add_compile_options(/W0)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.11.0)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file (GLOB test_sources "./source/*.h")

add_executable(ArborMaster_test source/ArborMaster_test.cpp ${test_sources} )
target_link_libraries(ArborMaster_test PRIVATE ArborMaster_lib ArborMaster_adapter GTest::gtest_main GTest::gmock GTest::gmock_main)
target_compile_features(ArborMaster_test PRIVATE 
	cxx_std_20
)

include(GoogleTest)
gtest_discover_tests(ArborMaster_test)


# ---- End-of-file commands ----

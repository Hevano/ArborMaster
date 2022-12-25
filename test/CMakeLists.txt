# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(ArborMasterTests LANGUAGES CXX)

# ---- Tests ----

add_executable(ArborMaster_test source/ArborMaster_test.cpp)
target_link_libraries(ArborMaster_test PRIVATE ArborMaster_lib)
target_compile_features(ArborMaster_test PRIVATE cxx_std_20)

add_test(NAME ArborMaster_test COMMAND ArborMaster_test)

# ---- End-of-file commands ----

add_folders(Test)

file(GLOB_RECURSE all_tests RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc *.h)
add_executable(yukcorlib_test ${all_tests})
target_link_libraries(yukcorlib_test yukcorlib gtest)
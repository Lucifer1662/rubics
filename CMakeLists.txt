# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.14)

project ("context")

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-D_HAS_AUTO_PTR_ETC=1)


include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


include(GoogleTest)
enable_testing()
add_executable(tests test.cc)
target_link_libraries(tests PUBLIC GTest::gtest_main)
gtest_discover_tests(tests)
add_test(test tests COMMAND tests)

add_executable(decimal main.cc)



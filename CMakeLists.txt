cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b10fad38c4026a29ea6561ab15fc4818170d1c10.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(firstTest test1.cpp)
target_link_libraries(firstTest GTest::gtest_main)

add_executable(example1 example1.cpp)
target_link_libraries(example1 GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(firstTest)
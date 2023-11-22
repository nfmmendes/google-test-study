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

add_executable(example2 example2.cpp)
target_link_libraries(example2 GTest::gtest_main)

add_executable(example3 example3.cpp)
target_link_libraries(example3 GTest::gtest_main)

add_executable(example4 example4.cpp)
target_link_libraries(example4 GTest::gtest_main)

add_executable(example5 example5.cpp)
target_link_libraries(example5 GTest::gtest_main)

add_executable(example6 example6.cpp)
target_link_libraries(example6 GTest::gtest_main)

add_executable(example7 example7.cpp)
target_link_libraries(example7 GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(firstTest)
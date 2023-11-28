cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest-lib)
add_library(GTest::GTest INTERFACE IMPORTED)
add_library(GTest::GMock INTERFACE IMPORTED)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

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

add_executable(example8 example8.cpp)
target_link_libraries(example8 GTest::gtest_main GTest::gmock_main)

add_executable(apiTest Api-client/main.cpp Api-client/HttpClientInterface.h Api-Client/AlrightAPI.h)
target_link_libraries(apiTest GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(firstTest)
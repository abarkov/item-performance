cmake_minimum_required(VERSION 3.10)

project(ItemPerformance)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test test.cc item_test.cc item_func.cc)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory(benchmark)

add_executable(gbench gbench.cc)
target_link_libraries(gbench benchmark::benchmark)

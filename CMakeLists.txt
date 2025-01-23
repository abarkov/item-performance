cmake_minimum_required(VERSION 3.10)

project(ItemPerformance)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test decimal.c dtoa.cc test.cc item_test.cc item_func.cc vm.cc)
set_target_properties(test PROPERTIES COMPILE_FLAGS "-DHAVE_NULL_VALUE")

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory(benchmark)

add_executable(gbench gbench.cc decimal.c dtoa.c item_test.cc item_func.cc vm.cc)
target_link_libraries(gbench benchmark::benchmark)
set_target_properties(gbench PROPERTIES COMPILE_FLAGS "-DHAVE_NULL_VALUE")

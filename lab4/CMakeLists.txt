cmake_minimum_required(VERSION 3.10)

project(iterator_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(iterator_lib INTERFACE)
target_include_directories(iterator_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(iterator_tests tests/iterator.cpp)
target_link_libraries(iterator_tests PRIVATE iterator_lib)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.1  
)
FetchContent_MakeAvailable(Catch2)

target_link_libraries(iterator_tests PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME IteratorTests COMMAND iterator_tests)
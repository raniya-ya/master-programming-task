cmake_minimum_required(VERSION 3.10)

project(typemap_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(typemap_lib INTERFACE)
target_include_directories(typemap_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(typemap_tests tests/type_map.cpp)
target_link_libraries(typemap_tests PRIVATE typemap_lib)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.1  
)
FetchContent_MakeAvailable(Catch2)

target_link_libraries(typemap_tests PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME TypemapTests COMMAND typemap_tests)
cmake_minimum_required(VERSION 3.10)

project(variant_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(variant_lib INTERFACE)
target_include_directories(variant_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(variant_tests tests/variant.cpp)
target_link_libraries(variant_tests PRIVATE variant_lib)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.1  
)
FetchContent_MakeAvailable(Catch2)

target_link_libraries(variant_tests PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME VariantTests COMMAND variant_tests)
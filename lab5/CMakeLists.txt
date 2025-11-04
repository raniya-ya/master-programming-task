cmake_minimum_required(VERSION 3.10)
project(variant)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost QUIET)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

add_library(variant INTERFACE)
target_include_directories(variant INTERFACE include)

file(GLOB TESTS tests/*.cpp)
add_executable(tests_variant ${TESTS})
target_link_libraries(tests_variant variant Catch2::Catch2WithMain)

# Если Boost найден - линкуем его
if(Boost_FOUND)
    target_link_libraries(tests_variant Boost::boost)
    message(STATUS "Boost found, linking...")
else()
    message(WARNING "Boost not found, building without Boost features")
endif()
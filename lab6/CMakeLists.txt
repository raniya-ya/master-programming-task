cmake_minimum_required(VERSION 3.10)
project(lab8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Catch2 REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(tests
    tests/json.cpp
    tests/quoted_string.cpp
)

target_link_libraries(tests
    PRIVATE
        Catch2::Catch2WithMain 
        ${Boost_LIBRARIES}
)

enable_testing()
add_test(NAME AllTests COMMAND tests)
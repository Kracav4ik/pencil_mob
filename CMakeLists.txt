cmake_minimum_required(VERSION 3.5)
project(pencil_mob)

add_definitions(-std=c++11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(
        NAME pencil_mob_tests
        COMMAND pencil_mob_tests
)

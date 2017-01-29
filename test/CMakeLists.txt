set(gtest_disable_pthreads on) # needed in MinGW?
add_subdirectory(googletest-release-1.8.0)

add_executable(pencil_mob_tests transport_check.cpp)

target_link_libraries(pencil_mob_tests gtest gtest_main pencil_mob_common)

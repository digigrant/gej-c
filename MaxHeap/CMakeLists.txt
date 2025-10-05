cmake_minimum_required(VERSION 3.28)

project(gej_MaxHeap
    LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_library(gej_MaxHeap STATIC gej_MaxHeap.c)

option(BUILD_TESTS "Build tests for MaxHeap lib" OFF)
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
    )
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(gej_MaxHeap_tests gej_MaxHeap_tests.cpp)
    add_dependencies(gej_MaxHeap_tests gej_MaxHeap)
    target_link_libraries(gej_MaxHeap_tests PRIVATE
        gej_MaxHeap
        GTest::gtest
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(gej_MaxHeap_tests)
endif()

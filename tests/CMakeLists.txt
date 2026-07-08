cmake_minimum_required(VERSION 3.14)

project(tmech_symdiff_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest: prefer a system-installed copy (works offline); otherwise, or when
# TMECH_DOWNLOAD_GTEST is ON, fetch a pinned release via FetchContent.
set(TMECH_GTEST_MAIN "")
if(NOT TMECH_DOWNLOAD_GTEST)
    find_package(GTest QUIET)
    if(GTest_FOUND)
        set(TMECH_GTEST_MAIN GTest::gtest_main)
        message(STATUS "tmech tests: using system GoogleTest (${GTest_VERSION})")
    endif()
endif()

if(NOT TMECH_GTEST_MAIN)
    message(STATUS "tmech tests: fetching GoogleTest v1.15.2 via FetchContent")
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    set(TMECH_GTEST_MAIN gtest_main)
endif()

include(GoogleTest)
macro(add_tmech_test TARGET_NAME)
    add_executable(${TARGET_NAME} ${ARGN})
    target_link_libraries(${TARGET_NAME} tmech ${TMECH_GTEST_MAIN})
    add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
    #gtest_discover_tests(${TARGET_NAME})
endmacro()

add_tmech_test(tmech_test main.cpp)

# Make the library's randn() reproducible across runs so test failures can be
# reproduced from CI logs (does not affect the installed library).
target_compile_definitions(tmech_test PRIVATE TMECH_DETERMINISTIC_RANDN)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tmech_test PRIVATE  -Wall -Wuninitialized -Werror=uninitialized -Wpedantic -Werror -Wextra -pedantic -Wconversion -Wno-error=comment)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(tmech_test PRIVATE /WX /bigobj)
endif()

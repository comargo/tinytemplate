cmake_minimum_required(VERSION 2.8)
project(tinytemplate_tests)

set(BUILD_GMOCK OFF CACHE BOOL "Builds the googlemock subproject" FORCE)
set(BUILD_GTEST ON CACHE BOOL "Builds the googletest subproject" FORCE)

add_subdirectory(googletest)

set(${PROJECT_NAME}_SOURCES
	tinytemplate_tests.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -pedantic -Wextra)
target_link_libraries(${PROJECT_NAME} tinytemplate gtest_main)


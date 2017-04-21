cmake_minimum_required(VERSION 2.8)
project(tinytemplate_tests)
include(PVS-Studio)

set(BUILD_GMOCK OFF CACHE BOOL "Builds the googlemock subproject" FORCE)
set(BUILD_GTEST ON CACHE BOOL "Builds the googletest subproject" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib." FORCE)

add_subdirectory(googletest)
if(MSVC_VERSION LESS 1800)
    target_compile_definitions(gtest
        PUBLIC GTEST_HAS_TR1_TUPLE=0)
endif()

set(${PROJECT_NAME}_SOURCES
	tinytemplate_tests.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME}
        PRIVATE -Wall -pedantic -Wextra)
endif()
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} tinytemplate gtest_main)

# PVS-Studio analyze target
pvs_studio_add_target(TARGET analyze_${PROJECT_NAME}
                FORMAT tasklist
                OUTPUT FORMAT errorfile
                PREPROCESSOR gcc
                LOG "${PROJECT_BINARY_DIR}/target.plog"
                ANALYZE ${PROJECT_NAME}
		CXX_FLAGS "-I$<TARGET_PROPERTY:tinytemplate,INTERFACE_INCLUDE_DIRECTORIES>"
		C_FLAGS "-I$<TARGET_PROPERTY:tinytemplate,INTERFACE_INCLUDE_DIRECTORIES>"
                )

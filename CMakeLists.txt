cmake_minimum_required(VERSION 2.8)
project(tinytemplate)

option(tinytemplate_build_tests "Build tinytemplate tests" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

#Load modules
include(PVS-Studio)

set(${PROJECT_NAME}_SOURCES
	tinytemplate.cpp
)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME}
        PRIVATE -Wall -pedantic -Wextra)
endif()
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

if(tinytemplate_build_tests)
    add_subdirectory(tests)
endif()

# PVS-Studio analyze target
pvs_studio_add_target(TARGET analyze_${PROJECT_NAME}
                FORMAT tasklist
                OUTPUT FORMAT errorfile
                PREPROCESSOR gcc
                LOG "${PROJECT_BINARY_DIR}/target.plog"
                ANALYZE ${PROJECT_NAME}
                )

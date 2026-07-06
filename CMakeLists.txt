cmake_minimum_required(VERSION 3.24)

project(EveAdultEngine
    VERSION 0.1.0
    DESCRIPTION "Open-source engine for mature, story-driven, character-focused experiences"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(EveOptions)
include(EveCompilerWarnings)
include(EveDependencies)

enable_testing()

add_subdirectory(engine)
add_subdirectory(tests)

if(EVE_BUILD_EDITOR)
    add_subdirectory(apps/editor)
endif()

if(EVE_BUILD_EXAMPLE_SCENE)
    add_subdirectory(apps/example_scene)
endif()

if(EVE_ENABLE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

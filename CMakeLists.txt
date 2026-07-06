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

if(EVE_BUILD_WARDROBE_SLICE OR EVE_BUILD_TESTS)
    add_subdirectory(apps/wardrobe_slice)
endif()

if(EVE_BUILD_ANIME_DEMO OR EVE_BUILD_TESTS)
    add_subdirectory(apps/anime_wardrobe_demo)
endif()

if(EVE_BUILD_RAINY_DEMO OR EVE_BUILD_TESTS)
    add_subdirectory(apps/rainy_evening_demo)
endif()

if(EVE_BUILD_MOVIE_DEMO OR EVE_BUILD_TESTS)
    add_subdirectory(apps/rainy_movie_night_demo)
endif()

if(EVE_BUILD_CREATOR_STUDIO OR EVE_BUILD_TESTS)
    add_subdirectory(apps/creator_studio)
endif()

if(EVE_BUILD_WEB_CHARACTER_VIEWER OR EVE_BUILD_TESTS)
    add_subdirectory(apps/web_character_viewer)
endif()

if(EVE_BUILD_PERSONAL_EXPORT OR EVE_BUILD_TESTS)
    add_subdirectory(apps/personal_export)
endif()

if(EVE_BUILD_APARTMENT_LIFE_DEMO OR EVE_BUILD_TESTS)
    add_subdirectory(apps/apartment_life_demo)
endif()

if(EVE_BUILD_RC1_RELEASE OR EVE_BUILD_TESTS)
    add_subdirectory(apps/rc1_release)
endif()

if(EVE_ENABLE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

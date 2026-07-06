option(EVE_BUILD_EDITOR "Build the EVE editor application" ON)
option(EVE_BUILD_EXAMPLE_SCENE "Build apartment character example scene" ON)
option(EVE_BUILD_WARDROBE_SLICE "Build wardrobe vertical slice application" ON)
option(EVE_BUILD_ANIME_DEMO "Build anime wardrobe demo application" ON)
option(EVE_BUILD_TESTS "Build automated tests" ON)
option(EVE_ENABLE_VULKAN_VALIDATION "Enable Vulkan validation layers" OFF)
option(EVE_ENABLE_BENCHMARKS "Build subsystem benchmarks" OFF)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build: Debug Release RelWithDebInfo MinSizeRel" FORCE)

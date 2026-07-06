add_library(eve_polish STATIC
    src/performance_profiler.cpp
    src/graphics_polish.cpp
    src/camera_polish.cpp
    src/animation_polish.cpp
    src/wardrobe_polish.cpp
    src/apartment_atmosphere.cpp
    src/validation_gate.cpp
    src/bug_tracker.cpp
    src/rc1_release.cpp
    src/dialogue_polish.cpp
)

add_library(eve::polish ALIAS eve_polish)

target_include_directories(eve_polish
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_polish
    PUBLIC
        eve::core
        eve::render
        eve::human
        eve::animation
        eve::interaction
        eve::world
        eve::build
        eve::platform
        eve::ai
        eve::content
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_polish PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_polish)
eve_set_compile_definitions(eve_polish)

add_library(eve_gameplay STATIC
    src/apartment_life_session.cpp
)

add_library(eve::gameplay ALIAS eve_gameplay)

target_include_directories(eve_gameplay
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_gameplay
    PUBLIC
        eve::core
        eve::content
        eve::ai
        eve::scene
        eve::interaction
        eve::platform
        eve::build
        eve::human
        eve::animation
        eve::world
        eve::polish
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_gameplay PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_gameplay)
eve_set_compile_definitions(eve_gameplay)

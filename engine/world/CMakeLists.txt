add_library(eve_world STATIC
    src/apartment_types.cpp
    src/interaction_system.cpp
    src/apartment.cpp
    src/simulation.cpp
    src/persistence.cpp
    src/anime_room_lighting.cpp
)

add_library(eve::world ALIAS eve_world)

target_include_directories(eve_world
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_world
    PUBLIC
        eve::core
        eve::ecs
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_world PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_world)
eve_set_compile_definitions(eve_world)

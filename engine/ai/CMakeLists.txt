add_library(eve_ai STATIC
    src/wardrobe.cpp
    src/relationship.cpp
    src/schedule.cpp
    src/social_ai.cpp
    src/romance_hooks.cpp
)

add_library(eve::ai ALIAS eve_ai)

target_include_directories(eve_ai
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_ai
    PUBLIC
        eve::core
        eve::ecs
        eve::world
)

target_compile_features(eve_ai PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_ai)
eve_set_compile_definitions(eve_ai)

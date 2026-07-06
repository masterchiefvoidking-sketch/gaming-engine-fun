add_library(eve_scripting STATIC
    src/script_engine.cpp
    src/python_bridge.cpp
)

add_library(eve::scripting ALIAS eve_scripting)

target_include_directories(eve_scripting
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_scripting
    PUBLIC
        eve::core
)

target_compile_features(eve_scripting PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_scripting)
eve_set_compile_definitions(eve_scripting)

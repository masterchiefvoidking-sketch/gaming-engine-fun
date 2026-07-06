add_library(eve_core STATIC
    src/memory/allocator.cpp
    src/logging/logger.cpp
    src/math/math.cpp
    src/reflection/reflection.cpp
    src/serialization/serializer.cpp
    src/filesystem/filesystem.cpp
    src/threading/job_system.cpp
    src/plugin/plugin_manager.cpp
)

add_library(eve::core ALIAS eve_core)

target_include_directories(eve_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(eve_core PUBLIC cxx_std_20)

eve_set_compile_warnings(eve_core)
eve_set_compile_definitions(eve_core)

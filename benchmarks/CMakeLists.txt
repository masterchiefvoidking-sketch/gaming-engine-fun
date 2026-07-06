cmake_minimum_required(VERSION 3.24)

function(eve_add_benchmark target)
    add_executable(${target} ${ARGN})
    target_compile_features(${target} PRIVATE cxx_std_20)
    eve_set_compile_warnings(${target})
endfunction()

eve_add_benchmark(eve_benchmark_character_creator benchmark_character_creator.cpp)
target_link_libraries(eve_benchmark_character_creator PRIVATE eve::human)

eve_add_benchmark(eve_benchmark_inspection_camera benchmark_inspection_camera.cpp)
target_link_libraries(eve_benchmark_inspection_camera PRIVATE eve::render)

eve_add_benchmark(eve_benchmark_save_load benchmark_save_load.cpp)
target_link_libraries(eve_benchmark_save_load PRIVATE eve::platform)

function(eve_set_compile_warnings target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Wshadow -Wconversion>
    )
endfunction()

function(eve_set_compile_definitions target)
    target_compile_definitions(${target} PRIVATE
        EVE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        EVE_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        EVE_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        $<$<CONFIG:Debug>:EVE_DEBUG=1>
        $<$<BOOL:${EVE_ENABLE_VULKAN_VALIDATION}>:EVE_VULKAN_VALIDATION=1>
    )
endfunction()

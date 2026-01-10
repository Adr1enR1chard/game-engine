# Unified compiler warnings configuration for all targets.
# Usage: include(cmake/CompilerWarnings.cmake) then call enable_strict_warnings(<target>).

option(ENGINE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

function(enable_strict_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /permissive-
            /Zc:__cplusplus
            /EHsc
            $<$<BOOL:${ENGINE_WARNINGS_AS_ERRORS}>:/WX>
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            -Wcast-align
            -Woverloaded-virtual
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
            -Wimplicit-fallthrough
            $<$<BOOL:${ENGINE_WARNINGS_AS_ERRORS}>:-Werror>
        )
    endif()
endfunction()

function(add_asset_pipeline)
    set(options)
    set(oneValueArgs
        TARGET
        SOURCE_DIR
        RUNTIME_DIR
        COMPILER
    )
    set(multiValueArgs
        EXTENSIONS
    )

    cmake_parse_arguments(ASSETS
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    if(NOT ASSETS_TARGET)
        message(FATAL_ERROR "add_asset_pipeline: TARGET is required")
    endif()

    if(NOT ASSETS_SOURCE_DIR)
        message(FATAL_ERROR "add_asset_pipeline: SOURCE_DIR is required")
    endif()

    if(NOT ASSETS_RUNTIME_DIR)
        message(FATAL_ERROR "add_asset_pipeline: RUNTIME_DIR is required")
    endif()

    if(NOT ASSETS_COMPILER)
        message(FATAL_ERROR "add_asset_pipeline: COMPILER is required")
    endif()

    # ------------------------------------------------------------
    # Discover assets
    # ------------------------------------------------------------
    set(GLOBS "")
    foreach(ext IN LISTS ASSETS_EXTENSIONS)
        list(APPEND GLOBS "${ASSETS_SOURCE_DIR}/*.${ext}")
    endforeach()

    file(GLOB_RECURSE ASSET_FILES
        CONFIGURE_DEPENDS
        ${GLOBS}
    )

    # ------------------------------------------------------------
    # Per-asset compilation
    # ------------------------------------------------------------
    set(ASSET_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets_stamps/${ASSETS_TARGET}")
    set(ASSET_OUTPUTS "")

    foreach(ASSET_FILE IN LISTS ASSET_FILES)

        file(RELATIVE_PATH REL_PATH
            "${ASSETS_SOURCE_DIR}"
            "${ASSET_FILE}"
        )

        get_filename_component(REL_DIR "${REL_PATH}" DIRECTORY)
        get_filename_component(REL_NAME "${REL_PATH}" NAME)

        set(STAMP_FILE
            "${ASSET_BUILD_DIR}/${REL_DIR}/${REL_NAME}.stamp"
        )

        list(APPEND ASSET_OUTPUTS "${STAMP_FILE}")

        add_custom_command(
            OUTPUT "${STAMP_FILE}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory
                    "${ASSET_BUILD_DIR}/${REL_DIR}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory
                    "${ASSETS_RUNTIME_DIR}/${REL_DIR}"
            COMMAND "${ASSETS_COMPILER}"
                    "${ASSET_FILE}"
                    "${ASSETS_RUNTIME_DIR}/${REL_DIR}"
            COMMAND "${CMAKE_COMMAND}" -E touch "${STAMP_FILE}"
            DEPENDS "${ASSET_FILE}"
            VERBATIM
        )

    endforeach()

    # ------------------------------------------------------------
    # Aggregate target
    # ------------------------------------------------------------
    set(PIPELINE_TARGET "${ASSETS_TARGET}_assets")

    add_custom_target("${PIPELINE_TARGET}"
        DEPENDS ${ASSET_OUTPUTS}
    )

    add_dependencies("${ASSETS_TARGET}" "${PIPELINE_TARGET}")

endfunction()

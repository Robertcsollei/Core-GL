# Web-specific build configuration for TerraKit

# This file contains Emscripten-specific settings and utilities
# Include this when building for WebAssembly

# Emscripten build settings
if(TERRAKIT_BUILD_WASM)
    # Common Emscripten flags for all targets
    set(EMSCRIPTEN_COMMON_FLAGS
        "SHELL:-s USE_SDL=2"
        "SHELL:-s USE_WEBGL2=1"
        "SHELL:-s MIN_WEBGL_VERSION=2"
        "SHELL:-s MAX_WEBGL_VERSION=2"
        "SHELL:-s FULL_ES3=1"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:-s INITIAL_MEMORY=67108864"  # 64MB
        "SHELL:-s EXPORTED_FUNCTIONS=['_main']"
        "SHELL:-s EXPORTED_RUNTIME_METHODS=['ccall']"
        "SHELL:-s NO_DISABLE_EXCEPTION_CATCHING"
    )

    # Development vs Release settings
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND EMSCRIPTEN_COMMON_FLAGS
            "SHELL:-s ASSERTIONS=1"
            "SHELL:-s GL_ASSERTIONS=1"
            "SHELL:-s SAFE_HEAP=1"
            "SHELL:-g"
        )
    else()
        list(APPEND EMSCRIPTEN_COMMON_FLAGS
            "SHELL:-O3"
            "SHELL:-s ASSERTIONS=0"
        )
    endif()

    # Function to apply web settings to a target
    function(configure_web_target TARGET_NAME)
        target_link_options(${TARGET_NAME} PRIVATE ${EMSCRIPTEN_COMMON_FLAGS})

        # Preload assets
        target_link_options(${TARGET_NAME} PRIVATE
            "SHELL:--preload-file ${CMAKE_SOURCE_DIR}/assets@/assets"
        )

        # Use custom HTML shell
        target_link_options(${TARGET_NAME} PRIVATE
            "SHELL:--shell-file ${CMAKE_SOURCE_DIR}/web/shell.html"
        )

        # Set output to HTML file
        set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".html")
    endfunction()
endif()
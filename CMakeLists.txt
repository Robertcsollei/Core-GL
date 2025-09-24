cmake_minimum_required(VERSION 3.21)
project(TerraKit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform detection
option(TERRAKIT_BUILD_WASM "Build for WebAssembly" OFF)

if(TERRAKIT_BUILD_WASM)
    # Auto-detect Emscripten toolchain if not already set
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)  
        # Try common Emscripten installation paths
        if(DEFINED ENV{EMSCRIPTEN})
            set(CMAKE_TOOLCHAIN_FILE "$ENV{EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake")
        elseif(EXISTS "${CMAKE_SOURCE_DIR}/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
            set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
        else() 
            message(FATAL_ERROR
                "WASM build requires Emscripten toolchain. Please:\n"
                "1. Install emsdk: git clone https://github.com/emscripten-core/emsdk.git\n"
                "2. Activate it: emsdk install latest && emsdk activate latest\n"
                "3. Run: emsdk_env.bat (or source emsdk_env.sh)\n"
                "4. Or manually set: -DCMAKE_TOOLCHAIN_FILE=path/to/Emscripten.cmake")
        endif()
    endif()

    # Verify we're actually using Emscripten 
    if(NOT EMSCRIPTEN)
        message(FATAL_ERROR "TERRAKIT_BUILD_WASM=ON but not using Emscripten toolchain")
    endif()

    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    message(STATUS "Building for WebAssembly with toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# vcpkg toolchain (only for non-WASM builds)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT TERRAKIT_BUILD_WASM)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

# Add subdirectories
add_subdirectory(vendor)
add_subdirectory(engine)
add_subdirectory(examples)

# Include web configuration if building for WASM
if(TERRAKIT_BUILD_WASM)
    add_subdirectory(web)
endif()
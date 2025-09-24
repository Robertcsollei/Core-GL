# Engine library CMakeLists.txt

# Find packages from vcpkg
find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

# CPR is desktop-only (not WASM compatible)
if(NOT TERRAKIT_BUILD_WASM)
    find_package(cpr CONFIG REQUIRED)
endif()

# Collect engine sources
file(GLOB_RECURSE ENGINE_SOURCES
     CONFIGURE_DEPENDS
     "src/*.cpp"
     "src/*.c"
)

file(GLOB_RECURSE ENGINE_HEADERS
     CONFIGURE_DEPENDS
     "src/*.h"
     "src/*.hpp"
     "include/*.h"
     "include/*.hpp"
)

# Create engine static library
add_library(TerraKitEngine STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

# Public include directories (what consumers of the library see)
target_include_directories(TerraKitEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_SOURCE_DIR}/vendor/imgui
        ${CMAKE_SOURCE_DIR}/vendor
    PRIVATE
        src
)

# Add ImGui sources explicitly
target_sources(TerraKitEngine PRIVATE  
    "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_sdl2.cpp"
)

# Link dependencies
target_link_libraries(TerraKitEngine
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        glad::glad
        glm::glm
)

# Link CPR only for desktop builds
if(NOT TERRAKIT_BUILD_WASM)
    target_link_libraries(TerraKitEngine PRIVATE cpr::cpr)
endif()

# C++ standard
set_property(TARGET TerraKitEngine PROPERTY CXX_STANDARD 20)
set_property(TARGET TerraKitEngine PROPERTY CXX_STANDARD_REQUIRED ON)


# IDE folder organization
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             FILES ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# MSVC specific settings
if(MSVC)
    target_compile_options(TerraKitEngine PRIVATE
        "$<$<CONFIG:Debug>:/Zi>"
        /diagnostics:caret
    )
endif()
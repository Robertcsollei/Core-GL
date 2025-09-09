cmake_minimum_required(VERSION 3.16)  # CONFIGURE_DEPENDS works reliably here
project(TerraKit LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(glad REQUIRED)

# Collect sources/headers automatically (recursively)
file(GLOB_RECURSE TERRAKIT_SOURCES
     CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.c"
     "${CMAKE_SOURCE_DIR}/src/*.cpp"
    )
file(GLOB_RECURSE TERRAKIT_HEADERS
     CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.h"
     "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

list(FILTER TERRAKIT_SOURCES EXCLUDE REGEX ".*/imgui/main\\.cpp$")

add_executable(TerraKit
  ${TERRAKIT_SOURCES}
  ${TERRAKIT_HEADERS}  
  app.rc
)

target_include_directories(
    TerraKit 
    PRIVATE 
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/vendor"
    "${CMAKE_SOURCE_DIR}/src/stb_image"
    "${CMAKE_SOURCE_DIR}/src/imgui"
)
target_link_libraries(TerraKit PRIVATE SDL2::SDL2 SDL2::SDL2main glad::glad)
set_property(TARGET TerraKit PROPERTY CXX_STANDARD 20)

# Nice folder view in IDEs (VS, CLion)
source_group(TREE "${CMAKE_SOURCE_DIR}/src"
             FILES ${TERRAKIT_SOURCES} ${TERRAKIT_HEADERS})

# Copy assets next to the built exe (so you can load "res/textures/test.png")
add_custom_command(TARGET TerraKit POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/src/res"
          "$<TARGET_FILE_DIR:TerraKit>/res"
  COMMENT "Copying assets to runtime dir"
)

if (MSVC)
  add_compile_options(
    "$<$<CONFIG:Debug>:/Zi>"
    /diagnostics:caret
  )
  add_link_options("$<$<CONFIG:Debug>:/DEBUG:FULL>")
endif()
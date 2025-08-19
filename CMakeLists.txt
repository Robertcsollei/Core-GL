cmake_minimum_required(VERSION 3.16)  # CONFIGURE_DEPENDS works reliably here
project(OpenGL_test LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(glad REQUIRED)

# Collect sources/headers automatically (recursively)
file(GLOB_RECURSE OPENGL_TEST_SOURCES
     CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.c"
     "${CMAKE_SOURCE_DIR}/src/*.cpp"
    )
file(GLOB_RECURSE OPENGL_TEST_HEADERS
     CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.h"
     "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

list(FILTER OPENGL_TEST_SOURCES EXCLUDE REGEX ".*/imgui/main\\.cpp$")

add_executable(OpenGL_test
  ${OPENGL_TEST_SOURCES}
  ${OPENGL_TEST_HEADERS}   # headers are optional, but nice for IDEs
  src/tests/TestMenu.cpp 
)

target_include_directories(
    OpenGL_test 
    PRIVATE 
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/vendor"
    "${CMAKE_SOURCE_DIR}/src/tests"
    "${CMAKE_SOURCE_DIR}/src/stb_image"
    "${CMAKE_SOURCE_DIR}/src/imgui"
)
target_link_libraries(OpenGL_test PRIVATE SDL2::SDL2 SDL2::SDL2main glad::glad)
set_property(TARGET OpenGL_test PROPERTY CXX_STANDARD 20)

# Nice folder view in IDEs (VS, CLion)
source_group(TREE "${CMAKE_SOURCE_DIR}/src"
             FILES ${OPENGL_TEST_SOURCES} ${OPENGL_TEST_HEADERS})

# Copy assets next to the built exe (so you can load "res/textures/test.png")
add_custom_command(TARGET OpenGL_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/src/res"
          "$<TARGET_FILE_DIR:OpenGL_test>/res"
  COMMENT "Copying assets to runtime dir"
)

if (MSVC)
  add_compile_options("$<$<CONFIG:Debug>:/Zi>")
  add_link_options("$<$<CONFIG:Debug>:/DEBUG:FULL>")
endif()
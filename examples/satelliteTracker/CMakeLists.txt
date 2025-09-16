# Satellite Tracker example

# Collect example sources
file(GLOB_RECURSE SATELLITE_TRACKER_SOURCES
     CONFIGURE_DEPENDS
     "*.cpp"
     "*.c"
)

file(GLOB_RECURSE SATELLITE_TRACKER_HEADERS
     CONFIGURE_DEPENDS
     "*.h"
     "*.hpp"
)

# Create executable
add_executable(SatelliteTracker
    ${SATELLITE_TRACKER_SOURCES}
    ${SATELLITE_TRACKER_HEADERS}
    ${CMAKE_SOURCE_DIR}/app.rc  # Windows resource file if it exists
)

# Link to engine
target_link_libraries(SatelliteTracker 
    PRIVATE 
    TerraKitEngine
)

# Include directories for example-specific code
target_include_directories(SatelliteTracker
    PRIVATE
    .
)

# C++ standard
set_property(TARGET SatelliteTracker PROPERTY CXX_STANDARD 20)

# Copy assets to build directory
add_custom_command(TARGET SatelliteTracker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:SatelliteTracker>/assets"
    COMMENT "Copying assets to runtime dir"
)

# IDE folder organization
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             FILES ${SATELLITE_TRACKER_SOURCES} ${SATELLITE_TRACKER_HEADERS})

# MSVC specific settings
if(MSVC)
    target_compile_options(SatelliteTracker PRIVATE
        "$<$<CONFIG:Debug>:/Zi>"
        /diagnostics:caret
    )
    target_link_options(SatelliteTracker PRIVATE
        "$<$<CONFIG:Debug>:/DEBUG:FULL>"
    )
endif()
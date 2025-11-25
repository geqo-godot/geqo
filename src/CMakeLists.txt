# Setup requirement for this build file.
CMAKE_MINIMUM_REQUIRED(VERSION 3.23 FATAL_ERROR)
# Setup project name.
PROJECT(geqo)
# Calculate target bit count if not given. 
# This is also used in GDExtension platform specifiers due to the possibility of
# having separate builds for multiple different platforms.
IF (NOT DEFINED BITS)
  MATH(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
ENDIF()
# Set TARGET variable so we don't need to use the monstrosity below repeatedly.
SET(TARGET ${PROJECT_NAME}.${CMAKE_SYSTEM_NAME}.${CMAKE_BUILD_TYPE}.${BITS})
# Make it lower case.
STRING(TOLOWER ${TARGET} TARGET)
# Find all cpp files and put those in SOURCES variable.
FILE(GLOB_RECURSE SOURCES *.c**)
# Create build target from the name and sources we setup above.
ADD_LIBRARY(${TARGET} SHARED ${SOURCES})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES
  PREFIX ""
)
# Windows export
IF(WIN32)
  TARGET_COMPILE_DEFINITIONS(${TARGET} PRIVATE GDEXTENSION_LIBRARY_EXPORTS)
ENDIF()
# Add godot::cpp as private dependency.
TARGET_LINK_LIBRARIES(${TARGET} PRIVATE godot::cpp)
# If we are using Visual Studio compiler, add some options.
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # Enable C++17 support.
  TARGET_COMPILE_OPTIONS(${TARGET} PRIVATE /std:c++17)
  IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    # If target is debug build, setup proper runtime library (mainly to match godot-cpp one).
    SET(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
  ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)
ENDIF()
# Create a proper GDExtension configuration filename.
STRING(TOLOWER "${PROJECT_NAME}.gdextension" GDEXTENSION_CONFIGURATION_FILE)
# Create entry point name from project name.
STRING(TOLOWER ${PROJECT_NAME} GDEXTENSION_ENTRY_POINT)
# Create GDExtension configuration file from our configuration template.
CONFIGURE_FILE(gdextension.in ${GDEXTENSION_CONFIGURATION_FILE})
# Install into addons/geqo/bin
INSTALL(
    TARGETS ${TARGET}
    RUNTIME DESTINATION "addons/geqo/bin"
    LIBRARY DESTINATION "addons/geqo/bin"
    ARCHIVE DESTINATION "addons/geqo/bin"
)

# Install geqo.gdextension into addons/geqo
INSTALL(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${GDEXTENSION_CONFIGURATION_FILE}"
    DESTINATION "addons/geqo"
)

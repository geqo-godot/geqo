# Setup requirement for this build file.
CMAKE_MINIMUM_REQUIRED(VERSION 3.23 FATAL_ERROR)
# Add include path of godot-cpp
INCLUDE_DIRECTORIES(godot-cpp)
# Setup project name.
PROJECT(geqo)
# Add godot-cpp as part of this multi-project build.
ADD_SUBDIRECTORY(godot-cpp)
ADD_SUBDIRECTORY(src)
#  Disabling the test extension, it is retained as an example for now
#ADD_SUBDIRECTORY(gdextension_test)

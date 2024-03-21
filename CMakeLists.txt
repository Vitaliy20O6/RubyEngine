cmake_minimum_required(VERSION 3.29)

set(PROJECT_NAME RubyEngine)

add_subdirectory(RubyEngineCore)
add_subdirectory(RubyEngineEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RubyEngineEditor)
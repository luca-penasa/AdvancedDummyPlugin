cmake_minimum_required(VERSION 2.8)

#REPLACE ALL 'DUMMY' OCCURENCES BY YOUR PLUGIN NAME
#AND ADAPT THE CODE BELOW TO YOUR OWN NEEDS!

option( INSTALL_ADVANCED_DUMMY_PLUGIN_PLUGIN "Check to install ADVANCED_DUMMY_PLUGIN plugin" OFF )

if (INSTALL_ADVANCED_DUMMY_PLUGIN_PLUGIN)

#CloudCompare 'DUMMY' plugin
project( ADVANCED_DUMMY_PLUGIN_PLUGIN )

#load necessary libraries (see qPCV for an example)
#add_subdirectory (LIB1)

#if the plugin is an 'OpenGL filter', uncomment the line below
#set( CC_OPENGL_FILTER ON BOOL)
include( ../CMakePluginTpl.cmake )

#set dependencies to necessary libraries (see qPCV for an example)
#target_link_libraries( ${PROJECT_NAME} LIB1 )
#include_directories( ${LIB1_INCLUDE_DIR} )

endif()

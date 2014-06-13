# suggest running with 
#   gcc 4.6 : -DCMAKE_CXX_FLAGS="-std=c++0x"
#   gcc 4.8 : -DCMAKE_CXX_FLAGS="-std=c++11"

cmake_minimum_required(VERSION 2.8)

project (kwargs)
set( ${CMAKE_PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${CMAKE_PROJECT_NAME}_VERSION_MINOR 1 )
set( ${CMAKE_PROJECT_NAME}_VERSION_BUGFIX 0 )

# adds the project-specific cmake module directory cmake/Modules to the cmake
# search path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories(include)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)

include("TargetDoc" OPTIONAL)

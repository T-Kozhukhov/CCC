cmake_minimum_required(VERSION 3.1)
project(CCC) #project name
set(CMAKE_BUILD_TYPE Release) #force release build

#normal stuff
include_directories(include) #add headers
file(GLOB SOURCES "src/*.cpp") #add source files

#GSL
find_package(GSL REQUIRED) #add GSL libraries

#VTK
find_package(VTK REQUIRED) #add VTK libs
include(${VTK_USE_FILE})

#generate executable
add_executable(CCC ${SOURCES})

#link libraries
target_link_libraries(CCC GSL::gsl GSL::gslcblas) #link GSL libs
target_link_libraries(CCC ${VTK_LIBRARIES}) #link VTK libs

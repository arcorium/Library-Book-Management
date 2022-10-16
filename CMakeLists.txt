# CMakeList.txt : CMake project for library_management, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("library_management")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(thirdparty/tomlpp)
add_subdirectory(thirdparty/ftxui)

include_directories(src/)

set(VZ_SOURCES 
	src/main.cpp
	  "src/system/application.cpp")

set(VZ_HEADERS
	src/utility.h
	 "src/system/application.h" "src/system/user_interface.h" "src/interface/ftxui_interface.h")
# Add source to this project's executable.
add_executable (library_management  ${VZ_HEADERS} ${VZ_SOURCES})

target_link_libraries(library_management PUBLIC tomlplusplus_tomlplusplus screen dom component)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET library_management PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

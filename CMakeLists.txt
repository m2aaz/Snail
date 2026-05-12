cmake_minimum_required(VERSION 3.16)
project(sfml_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.cpp")

add_executable(sfml_app ${SOURCES})
target_include_directories(sfml_app PRIVATE include)

if(TARGET SFML::Graphics)
    target_link_libraries(sfml_app PRIVATE SFML::Graphics SFML::Window SFML::System)
elseif(TARGET sfml-graphics)
    target_link_libraries(sfml_app PRIVATE sfml-graphics sfml-window sfml-system)
else()
    message(FATAL_ERROR "No suitable SFML targets found")
endif()
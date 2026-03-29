cmake_minimum_required(VERSION 3.16)

project(sfml_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Homebrew SFML@2 path
set(SFML_DIR "/opt/homebrew/Cellar/sfml@2/2.6.2_1/lib/cmake/SFML")

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

# Add executable with all your source files
add_executable(sfml-app
    map.cpp
)

# Include directories
target_include_directories(sfml-app
    PRIVATE
        include                
        /opt/homebrew/Cellar/sfml@2/2.6.2_1/include  
)

# Link SFML libraries
target_link_libraries(sfml-app
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

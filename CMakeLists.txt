cmake_minimum_required(VERSION 3.28)
project(mvc)

set(CMAKE_CXX_STANDARD 17)

# Find the SFML library
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add the executable
add_executable(mvc main.cpp
)

# Link SFML libraries
target_link_libraries(mvc
        sfml-graphics
        sfml-window
        sfml-system
)

# Copy the arial.ttf font to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/arial.ttf DESTINATION ${CMAKE_BINARY_DIR})

# Set the working directory for the executable to the build directory
set_target_properties(mvc PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

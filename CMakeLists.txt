# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.12)

# Set the project name
project(YourProjectName)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Create a variable for the source files
set(SOURCES
    src/main.cpp
    src/Tokenizer.cpp
    src/Parser.cpp
)

# Create an executable
add_executable(main ${SOURCES})

# Include directories
target_include_directories(main PRIVATE include)

# Optionally, add other compiler flags or libraries if needed
# For example:
# target_compile_options(main PRIVATE -Wall -Wextra)
# target_link_libraries(main PRIVATE your_additional_library)

# Set the output directory for the executable
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

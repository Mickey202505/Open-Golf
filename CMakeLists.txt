cmake_minimum_required(VERSION 3.10)
project(BunkerTracer)

# Require C++17 for modern features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV (Ensure OpenCV is installed on your system)
find_package(OpenCV REQUIRED)

# Find libcurl (for downloading the Google Maps tile)
find_package(CURL REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})

# Create the executable
add_executable(bunker_tracer src/main.cpp)

# Link the libraries
target_link_libraries(bunker_tracer ${OpenCV_LIBS} ${CURL_LIBRARIES})

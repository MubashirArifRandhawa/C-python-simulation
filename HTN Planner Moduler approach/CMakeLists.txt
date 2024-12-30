cmake_minimum_required(VERSION 3.10)
project(AircraftSimulation)

# Set path for pybind11
set(pybind11_DIR "C:/Users/umar.shahid/AppData/Local/Programs/Python/Python312/Lib/site-packages/pybind11/share/cmake/pybind11")

# Explicitly set the Python executable and find the necessary components
set(Python3_EXECUTABLE "C:/Users/umar.shahid/AppData/Local/Programs/Python/Python312/python.exe")
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Find the pybind11 package
find_package(pybind11 REQUIRED)

# Set the SDL2 library directory
set(SDL2_LIB_DIR "D:/Cplusplus/HTN Planner Moduler approach/HTN Planner Moduler approach/SDL/SDL2-2.30.10/lib/x64")

# Set the SDL2 include directory
set(SDL2_INCLUDE_DIR "D:/Cplusplus/HTN Planner Moduler approach/HTN Planner Moduler approach/SDL/SDL2-2.30.10/include")

# Add SDL2 include directory
include_directories(${SDL2_INCLUDE_DIR})

# Add the source files and create the aircraft_simulation module
add_library(aircraft_simulation MODULE aircraft_simulation.cpp)

# Link pybind11 and SDL2 with the full path to the library files
target_link_libraries(aircraft_simulation PRIVATE pybind11::module)

# Full path to SDL2 and SDL2main libraries
target_link_libraries(aircraft_simulation PRIVATE 
    "${SDL2_LIB_DIR}/SDL2.lib"
    "${SDL2_LIB_DIR}/SDL2main.lib"
)

# Optional: Set the runtime path for SDL2.dll (if needed for runtime)
set_target_properties(aircraft_simulation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# If you are building in Debug, you might need to link the debug versions of SDL2 (e.g., SDL2d.lib for debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(aircraft_simulation PRIVATE 
        "${SDL2_LIB_DIR}/SDL2d.lib"  # Debug version of SDL2
    )
endif()


from setuptools import setup, Extension
import pybind11
import os

SDL2_INCLUDE_DIR = "D:\\Cplusplus\\HTN Planner Moduler approach\\HTN Planner Moduler approach\\SDL\\SDL2-2.30.10\\include"
SDL2_LIB_DIR = "D:\\Cplusplus\\HTN Planner Moduler approach\\HTN Planner Moduler approach\\SDL\\SDL2-2.30.10\\lib\\x64"

module = Extension(
    'aircraft_simulation',
    sources=['aircraft_simulation.cpp', 'Aircraft.cpp', 'Simulation.cpp'],
    include_dirs=[pybind11.get_include(), SDL2_INCLUDE_DIR],
    libraries=['SDL2', 'SDL2main'],
    library_dirs=[SDL2_LIB_DIR],
    extra_compile_args=['/std:c++17'],
    extra_link_args=['/LIBPATH:' + SDL2_LIB_DIR],  # Make sure to specify SDL2 lib path here
)

setup(
    name='AircraftSimulation',
    version='1.0',
    ext_modules=[module],
)

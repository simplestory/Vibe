cmake_minimum_required(VERSION 3.5)
project(VIBE LANGUAGES CXX)

include_directories(src/)
add_subdirectory(pybind11)
pybind11_add_module(VIBE src/vibe.cpp)
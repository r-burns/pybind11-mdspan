cmake_minimum_required(VERSION 3.14...3.19)

project(pymdspan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(PYTHON_PKGDIR pkgs CACHE STRING "Where to install python pkgs")

find_package(pybind11 2.5 CONFIG REQUIRED)
find_package(MDSpan CONFIG REQUIRED)

pybind11_add_module(pymdspan pymdspan.cpp)
target_link_libraries(pymdspan PRIVATE std::mdspan)

install(TARGETS pymdspan DESTINATION "${PYTHON_PKGDIR}")

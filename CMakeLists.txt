cmake_minimum_required(VERSION 3.14...3.19)

project(pymdspan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(PYTHON_PKGDIR pkgs CACHE STRING "Where to install python pkgs")

find_package(pybind11 2.5 CONFIG REQUIRED)
find_package(MDSpan CONFIG REQUIRED)

include(GNUInstallDirs)

add_library(pybind11-mdspan INTERFACE)
target_include_directories(pybind11-mdspan INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

cmake_minimum_required(VERSION 3.14...3.19)

project(pymdspan LANGUAGES CXX)

set(PYTHON_PKGDIR pkgs CACHE STRING "Where to install python pkgs")

find_package(pybind11 2.5 CONFIG REQUIRED)
find_package(mdspan CONFIG REQUIRED)

include(GNUInstallDirs)

add_library(pybind11-mdspan INTERFACE)
target_include_directories(pybind11-mdspan INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(PYMDSPAN_TEST_STANDARDS "14;17;20" CACHE STRING
    "C++ standards to build tests against")

include(CTest)
if(BUILD_TESTING)

    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    foreach(CMAKE_CXX_STANDARD ${PYMDSPAN_TEST_STANDARDS})
        add_subdirectory(tests "tests_cxx_std_${CMAKE_CXX_STANDARD}")
    endforeach()

endif()

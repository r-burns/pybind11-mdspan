set(suf ".cxx_std_${CMAKE_CXX_STANDARD}")
set(target "pymdspan${suf}")
set(test "pytests${suf}")

pybind11_add_module("${target}" pymdspan.cpp)
set_target_properties("${target}" PROPERTIES OUTPUT_NAME pymdspan)
target_link_libraries("${target}" PRIVATE std::mdspan pybind11-mdspan)

add_test(NAME "${test}" COMMAND pytest -ra -v ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties("${test}" PROPERTIES ENVIRONMENT
    "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}:$ENV{PYTHONPATH}")

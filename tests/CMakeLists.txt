pybind11_add_module(pymdspan pymdspan.cpp)
target_link_libraries(pymdspan PRIVATE std::mdspan pybind11-mdspan)

add_test(NAME pytests COMMAND pytest -ra -v ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(pytests PROPERTIES ENVIRONMENT
    "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}:$ENV{PYTHONPATH}")

#include <pybind11/pybind11.h>

#include "pybind11-mdspan.hpp"

namespace stdex = std::experimental;

using Extents1D = stdex::extents<stdex::dynamic_extent>;
using T = stdex::basic_mdspan<double, Extents1D, stdex::layout_right>;

void test(T a) {
    printf("1d span:\n");
    for (int i = 0; i < a.extent(0); i++) {
        printf("%g\n", a(i));
    }
}

using Extents2D = stdex::extents<stdex::dynamic_extent, stdex::dynamic_extent>;
using T2 = stdex::basic_mdspan<double, Extents2D, stdex::layout_right>;

void test2(T2 a) {
    printf("2d span:\n");
    for (int i = 0; i < a.extent(0); i++) {
        for (int j = 0; j < a.extent(1); j++) {
            printf("%g\n", a(i, j));
        }
    }
}

PYBIND11_MODULE(pymdspan, m) {
    m.def("test", test);
    m.def("test2", test2);
}

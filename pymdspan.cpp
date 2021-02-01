#include <pybind11/pybind11.h>
#include "pybind11-mdspan.hpp"

#include "log.h"

namespace stdex = std::experimental;

using Extents1D = stdex::extents<stdex::dynamic_extent>;
using Extents2D = stdex::extents<stdex::dynamic_extent, stdex::dynamic_extent>;
using Extents3D = stdex::extents<stdex::dynamic_extent, stdex::dynamic_extent, stdex::dynamic_extent>;
using Stride2D = stdex::layout_stride<stdex::dynamic_extent, stdex::dynamic_extent>;

using DenseSpan2D = stdex::basic_mdspan<int64_t, Extents2D, stdex::layout_right>;
using DenseSpan3D = stdex::basic_mdspan<int64_t, Extents3D, stdex::layout_right>;

using StridedSpan2D = stdex::basic_mdspan<const int64_t, Extents2D, Stride2D>;

int64_t sum_int_2d(StridedSpan2D a) {
    int64_t sum = 0;
    for (int i = 0; i < a.extent(0); i++) {
        for (int j = 0; j < a.extent(1); j++) {
            sum += a(i, j);
        }
    }
    return sum;
}

int64_t sum_int_2d_dense(DenseSpan2D a) {
    int64_t sum = 0;
    for (int i = 0; i < a.extent(0); i++) {
        for (int j = 0; j < a.extent(1); j++) {
            sum += a(i, j);
        }
    }
    return sum;
}

int64_t sum_int_3d_dense(DenseSpan3D a) {
    int64_t sum = 0;
    for (int i = 0; i < a.extent(0); i++) {
        for (int j = 0; j < a.extent(1); j++) {
            for (int k = 0; k < a.extent(2); k++) {
                sum += a(i, j, k);
            }
        }
    }
    return sum;
}

PYBIND11_MODULE(pymdspan, m) {
    m.def("sum_int_2d", sum_int_2d);
    m.def("sum_int_2d_dense", sum_int_2d_dense);
    m.def("sum_int_3d_dense", sum_int_3d_dense);
}

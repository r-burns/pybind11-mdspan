#include <pybind11/pybind11.h>
#include <pybind11/mdspan.h>

namespace stdex = std::experimental;

using Extents1D = stdex::extents<stdex::dynamic_extent>;
using Extents2D = stdex::extents<stdex::dynamic_extent, stdex::dynamic_extent>;
using Extents3D = stdex::extents<stdex::dynamic_extent, stdex::dynamic_extent, stdex::dynamic_extent>;

template<typename T>
using DenseSpan2D = stdex::mdspan<T, Extents2D, stdex::layout_right>;
template<typename T>
using DenseSpan3D = stdex::mdspan<T, Extents3D, stdex::layout_right>;
template<typename T>
using StridedSpan2D = stdex::mdspan<T, Extents2D, stdex::layout_stride>;

template<typename T>
T sum_2d(StridedSpan2D<T> a) {
    T sum = 0;
    for (int i = 0; i < a.extent(0); i++) {
        for (int j = 0; j < a.extent(1); j++) {
            sum += a(i, j);
        }
    }
    return sum;
}

template<typename T>
T sum_2d_dense(DenseSpan2D<T> a) {
    T sum = 0;
    for (int i = 0; i < a.extent(0); i++) {
        for (int j = 0; j < a.extent(1); j++) {
            sum += a(i, j);
        }
    }
    return sum;
}

template<typename T>
T sum_3d_dense(DenseSpan3D<T> a) {
    T sum = 0;
    for (int i = 0; i < a.extent(0); i++) {
        for (int j = 0; j < a.extent(1); j++) {
            for (int k = 0; k < a.extent(2); k++) {
                sum += a(i, j, k);
            }
        }
    }
    return sum;
}

template<typename T>
T get_2d(StridedSpan2D<T> a, int i, int j) {
    return a(i, j);
}

PYBIND11_MODULE(pymdspan, m) {
    m.def("sum_int_2d", sum_2d<int64_t>);
    m.def("sum_int_2d_dense", sum_2d_dense<int64_t>);
    m.def("sum_float_2d_dense", sum_2d_dense<double>);
    m.def("sum_int_3d_dense", sum_3d_dense<int64_t>);
    m.def("get_int_2d", get_2d<int64_t>);

    using namespace pybind11::detail;
    {
        using arr = mdspan<int64_t, extents<2, dynamic_extent>, stdex::layout_stride>;
        m.def("get_int_2d_fixed2n", [](arr a, int i, int j) -> int64_t {
                return a(i, j);
        });
    }
    {
        using arr = mdspan<int64_t, extents<2, 2>, stdex::layout_stride>;
        m.def("get_int_2d_fixed22", [](arr a, int i, int j) -> int64_t {
                return a(i, j);
        });
    }
}

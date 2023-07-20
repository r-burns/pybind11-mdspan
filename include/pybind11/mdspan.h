#pragma once

#include <pybind11/numpy.h>

#if __has_include(<mdspan>)
#include <mdspan>
PYBIND11_NAMESPACE_BEGIN(PYBIND11_NAMESPACE)
PYBIND11_NAMESPACE_BEGIN(detail)
using std::mdspan;
using std::dynamic_extent;
using std::extents;
using std::layout_left;
using std::layout_right;
using std::layout_stride;
PYBIND11_NAMESPACE_END(detail)
PYBIND11_NAMESPACE_END(PYBIND11_NAMESPACE)
#elif __has_include(<experimental/mdspan>)
#include <experimental/mdspan>
PYBIND11_NAMESPACE_BEGIN(PYBIND11_NAMESPACE)
PYBIND11_NAMESPACE_BEGIN(detail)
using std::experimental::mdspan;
using std::experimental::dynamic_extent;
using std::experimental::extents;
using std::experimental::layout_left;
using std::experimental::layout_right;
using std::experimental::layout_stride;
PYBIND11_NAMESPACE_END(detail)
PYBIND11_NAMESPACE_END(PYBIND11_NAMESPACE)
#else
#error "Could not find mdspan header!"
#endif

#include "log.h"

PYBIND11_NAMESPACE_BEGIN(PYBIND11_NAMESPACE)
PYBIND11_NAMESPACE_BEGIN(detail)

// Convert to fully-unconstrained numpy-compatible mapping
// TODO do this without triggering -Wunused-value
template<typename Extents>
struct fully_dynamic_extents;
template<size_t... Extent>
struct fully_dynamic_extents<extents<Extent...>> {
    using type = extents<(Extent, dynamic_extent)...>;
};

// Converts a Numpy ndarray to a dynamic mdspan
template<typename Scalar, typename Extents, typename Layout, typename Access>
void ndarray_to_mdspan(array_t<Scalar>& arr, mdspan<Scalar, Extents, Layout, Access>& ret) {

    using Type = mdspan<Scalar, Extents, Layout, Access>;

    // Catch programmer errors
    static_assert(Extents::rank() == Extents::rank_dynamic(),
            "Extents must be fully dynamic");
    static_assert(!Type::mapping_type::is_always_contiguous(),
            "Layout must be fully strided");

    // Arrays for ndarray shape + stride layout
    std::array<size_t, Extents::rank()> extents_array;
    std::array<size_t, Extents::rank()> strides_array;
    for (size_t i = 0; i < Extents::rank(); i++) {
        // TODO will this ever happen?
        if (arr.strides(i) % sizeof(Scalar) != 0) {
            PYMDSPAN_LOG("Bad stride(%ld)=%ld is not a multiple of elem size (%lu)\n",
                    arr.strides(i), extents_array[i], sizeof(Scalar));
            exit(1);
        }
        extents_array[i] = arr.shape(i);
        strides_array[i] = arr.strides(i) / sizeof(Scalar);
    }

    const auto extents = typename Type::extents_type(extents_array);
    const auto mapping = typename Type::mapping_type(extents, strides_array);
    ret = Type(arr.mutable_data(), mapping);
}

// NB we won't implement all conversions here, just the ones we care about.
// Specifically, from fully-dynamic to some level of static specialization.
template<typename SpanA, typename SpanB>
_MDSPAN_CONSTEXPR_14 bool convert_to(const SpanA&, SpanB&);

template<typename Span>
_MDSPAN_CONSTEXPR_14 bool convert_to(const Span& a, Span& b) {
    b = a;
    return true;
}

template<
    typename Scalar, typename Extents, typename Access,
    typename DynExtents
>
_MDSPAN_CONSTEXPR_14 bool convert_to(
        const mdspan<Scalar, DynExtents, layout_stride, Access> a,
        mdspan<Scalar, Extents, layout_right, Access>& b) {

    using TypeA = mdspan<Scalar, DynExtents, layout_stride, Access>;
    using TypeB = mdspan<Scalar, Extents, layout_right, Access>;

    // Catch programmer errors
    static_assert(DynExtents::rank() == DynExtents::rank_dynamic(),
            "Extents must be fully dynamic");
    static_assert(!TypeA::mapping_type::is_always_contiguous(),
            "Layout must be fully strided");
    static_assert(Extents::rank() == Extents::rank_dynamic(),
            "Partial static extents not yet implemented :(");

    typename TypeB::mapping_type map(a.extents());
    for (size_t i = 0; i < Extents::rank(); i++) {
        if (b.static_extent(i) != dynamic_extent &&
            b.static_extent(i) != a.extent(i)) {
            PYMDSPAN_LOG("Static extent does not match\n");
            return false;
        }
        if (map.stride(i) != a.stride(i)) {
            PYMDSPAN_LOG("Stride does not match (got %ld, expected %ld)\n", a.stride(i), b.stride(i));
            return false;
        }
    }
    b = TypeB(a.data(), map);
    return true;
}

template<
    typename Scalar, typename Extents, typename Layout, typename Access,
    typename DynExtents,
    typename = typename std::enable_if<
        !std::is_same<
            mdspan<Scalar, DynExtents, layout_stride, Access>,
            mdspan<Scalar, Extents, Layout, Access>
        >::value
        && Extents::rank() != Extents::rank_dynamic()
    >::type
>
_MDSPAN_CONSTEXPR_14 bool convert_to(
        const mdspan<Scalar, DynExtents, layout_stride, Access> a,
        mdspan<Scalar, Extents, Layout, Access>& b) {

    using TypeA = mdspan<Scalar, DynExtents, layout_stride, Access>;
    using TypeB = mdspan<Scalar, Extents, Layout, Access>;

    // Catch programmer errors
    static_assert(DynExtents::rank() == DynExtents::rank_dynamic(),
            "Extents must be fully dynamic");
    static_assert(!TypeA::mapping_type::is_always_contiguous(),
            "Layout must be fully strided");

    std::array<size_t, Extents::rank()> strides;
    for (size_t i = 0; i < Extents::rank(); i++) {
        strides[i] = a.stride(i);
    }

    typename TypeB::mapping_type map{Extents{a.extents()}, strides};
    for (size_t i = 0; i < Extents::rank(); i++) {
        if (b.static_extent(i) != dynamic_extent &&
            b.static_extent(i) != a.extent(i)) {
            PYMDSPAN_LOG("Static extent does not match\n");
            return false;
        }
    }
    b = TypeB(a.data(), map);
    return true;
}

// The actual type caster - defined in terms of the above
// We trivially cast the ndarray to an underconstrained mdspan,
// and then check that the mdspan satisfies the type we actually want.
template<typename Scalar, typename Extents, typename Access, typename Layout>
struct type_caster<
    mdspan<Scalar, Extents, Layout, Access>
> {

private:
    using Type = mdspan<Scalar, Extents, Layout, Access>;
    using Mapping = typename Type::mapping_type;

    using DynExtents = typename fully_dynamic_extents<Extents>::type;
    using DynType = mdspan<Scalar, DynExtents, layout_stride, Access>;

    using Array = array_t<Scalar, array::forcecast>;

    Type ref;

public:
    static constexpr auto name = _("mdspan-from-ndarray");

    static constexpr bool need_writeable = !std::is_const<Scalar>::value;

    bool load(handle src, bool /* TODO conversion not supported */) {

        if (!isinstance<Array>(src)) {
            PYMDSPAN_LOG("Not an instance of array<%s>\n", typeid(Scalar).name());
            return false;
        }

        auto aref = reinterpret_borrow<Array>(src);

        if (!aref || (need_writeable && !aref.writeable())) {
            PYMDSPAN_LOG("Could not cast writeable array\n");
            return false;
        }

        if (Extents::rank() != aref.ndim()) {
            PYMDSPAN_LOG("Wrong rank (%ld vs %ld)\n", Extents::rank(), aref.ndim());
            return false;
        }

        DynType dref;
        ndarray_to_mdspan(aref, dref);

        if (!convert_to(dref, ref)) {
            PYMDSPAN_LOG("Could not specialize ndarray to specified mdspan\n");
            return false;
        }
        return true;
    }

    operator Type*() { return &ref; }
    operator Type&() { return ref; }

    template<typename U>
    using cast_op_type = pybind11::detail::cast_op_type<U>;
};

PYBIND11_NAMESPACE_END(detail)
PYBIND11_NAMESPACE_END(PYBIND11_NAMESPACE)

#pragma once

#include <pybind11/numpy.h>

#include <experimental/mdspan>
namespace pybind11 {
namespace detail {
using std::experimental::mdspan;
using std::experimental::basic_mdspan;
} // namespace pybind11
} // namespace detail

PYBIND11_NAMESPACE_BEGIN(PYBIND11_NAMESPACE)
PYBIND11_NAMESPACE_BEGIN(detail)

template<typename T, typename Extents, typename Layout, typename Access>
struct type_caster<
    basic_mdspan<T, Extents, Layout, Access>,
    enable_if_t<Extents::rank() == Extents::rank_dynamic()>
> {

    using Type = basic_mdspan<T, Extents, Layout, Access>;

private:
    using Array = array_t<T, array::forcecast | array::c_style>;
    Array copy_or_ref;

public:
    static constexpr auto name = _("mdspan-from-ndarray");

    std::unique_ptr<Type> ref;

    // TODO
    static constexpr bool need_writeable = false;

    bool load(handle src, bool convert) {

        bool need_copy = !isinstance<Array>(src);
        if (need_copy) {
            return false;
        }

        auto aref = reinterpret_borrow<Array>(src);

        if (!aref || (need_writeable && !aref.writeable())) {
            return false;
        }

        if (Extents::rank() != aref.ndim()) {
            return false;
        }

        std::array<ptrdiff_t, Extents::rank()> dyn_exts;
        for (int i = 0; i < Extents::rank(); i++) {
            dyn_exts[i] = aref.shape(i);
        }

        copy_or_ref = std::move(aref);

        ref.reset(new Type(data(copy_or_ref), dyn_exts));
        return true;
    }

    operator Type*() { return ref.get(); }
    operator Type&() { return *ref; }
    template<typename U>
    using cast_op_type = pybind11::detail::cast_op_type<U>;

private:
    T* data(Array& a) { return a.mutable_data(); }
};

PYBIND11_NAMESPACE_END(detail)
PYBIND11_NAMESPACE_END(PYBIND11_NAMESPACE)

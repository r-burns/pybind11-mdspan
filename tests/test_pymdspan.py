import numpy
import pytest

def test_sum_int_2d_trivial():
    from pymdspan import sum_int_2d

    a = numpy.array([[1, 2, 3, 4]])
    assert sum_int_2d(a) == 10

    a = numpy.array([[1],
                     [2],
                     [3],
                     [4]])
    assert sum_int_2d(a) == 10

    a = numpy.array([[1, 2],
                     [3, 4]])
    assert sum_int_2d(a) == 10

def test_sum_int_2d_strided():
    from pymdspan import sum_int_2d
    a = numpy.array([[1, 1, 0],
                     [1, 1, 0],
                     [0, 0, 0]])
    assert sum_int_2d(a[:2,:2]) == 4
    a = numpy.array([[0, 0, 1],
                     [0, 0, 1],
                     [1, 1, 1]])
    assert sum_int_2d(a[:2,:2]) == 0

    a = numpy.array([[1, 1, 0, 0],
                     [1, 1, 0, 0],
                     [0, 0, 0, 0]])
    assert sum_int_2d(a[:2,:2]) == 4
    a = numpy.array([[0, 0, 1, 1],
                     [0, 0, 1, 1],
                     [1, 1, 1, 1]])
    assert sum_int_2d(a[:2,:2]) == 0

def test_sum_int_2d_interleaved():
    from pymdspan import sum_int_2d
    a = numpy.array([[1, 0, 1, 0],
                     [1, 0, 1, 0],
                     [1, 0, 1, 0]])
    assert sum_int_2d(a[:,::2]) == 6
    a = numpy.array([[0, 1, 0, 1],
                     [0, 1, 0, 1],
                     [0, 1, 0, 1]])
    assert sum_int_2d(a[:,::2]) == 0

def test_sum_int_3d_dense():
    from pymdspan import sum_int_3d_dense
    a = numpy.array([[[1, 2],
                      [3, 4]],
                     [[1, 2],
                      [3, 4]]])
    assert sum_int_3d_dense(a) == 20

    # Make sure that strided access does not cast
    a = numpy.array([[[1, 1, 0],
                      [1, 1, 0],
                      [0, 0, 0]],
                     [[1, 1, 0],
                      [1, 1, 0],
                      [0, 0, 0]]])
    assert sum_int_3d_dense(a) == 8
    with pytest.raises(TypeError):
        sum_int_3d_dense(a[:2,:2,:])

def test_sum_int_2d_dense():
    from pymdspan import sum_int_2d_dense
    a = numpy.array([[1, 2],
                     [3, 4]])
    assert sum_int_2d_dense(a) == 10

    # Make sure that strided access does not cast
    a = numpy.array([[1, 1, 0],
                     [1, 1, 0],
                     [0, 0, 0]])
    assert sum_int_2d_dense(a) == 4
    with pytest.raises(TypeError):
        sum_int_2d_dense(a[:2,:2])

def test_sum_float_2d_dense():
    from pymdspan import sum_float_2d_dense
    a = numpy.array([[1, 2],
                     [3, 4]], dtype=float)
    assert sum_float_2d_dense(a) == 10

    # Make sure that strided access does not cast
    a = numpy.array([[1, 1, 0],
                     [1, 1, 0],
                     [0, 0, 0]], dtype=numpy.float64)
    assert sum_float_2d_dense(a) == 4
    with pytest.raises(TypeError):
        sum_float_2d_dense(a[:2,:2])

def test_get_2d():
    from pymdspan import get_int_2d
    a = numpy.arange(16).reshape((4, 4))
    assert get_int_2d(a, 0, 0) == 0
    assert get_int_2d(a, 1, 1) == 5
    assert get_int_2d(a, 0, 1) == 1
    assert get_int_2d(a, 1, 0) == 4

    assert get_int_2d(a[::2, ::2], 0, 0) == 0
    assert get_int_2d(a[::2, ::2], 1, 1) == 10
    assert get_int_2d(a[::2, ::2], 0, 1) == 2
    assert get_int_2d(a[::2, ::2], 1, 0) == 8

    assert get_int_2d(a[1:, 1:], 0, 0) == 5

def test_modify_2d():
    from pymdspan import get_int_2d, set_int_2d
    a = numpy.arange(16).reshape((4, 4))
    assert get_int_2d(a, 0, 0) == 0

    assert get_int_2d(a, 1, 1) == 5
    set_int_2d(a, 1, 1, 6)
    assert get_int_2d(a, 1, 1) == 6

    assert get_int_2d(a, 0, 1) == 1
    set_int_2d(a, 0, 1, 2)
    assert get_int_2d(a, 0, 1) == 2

    assert get_int_2d(a, 1, 0) == 4
    set_int_2d(a, 1, 0, 5)
    assert get_int_2d(a, 1, 0) == 5

    assert get_int_2d(a[::2, ::2], 0, 0) == 0
    set_int_2d(a[::2, ::2], 0, 0, 1)
    assert get_int_2d(a[::2, ::2], 0, 0) == 1

    assert get_int_2d(a[::2, ::2], 1, 1) == 10
    set_int_2d(a[::2, ::2], 1, 1, 11)
    assert get_int_2d(a[::2, ::2], 1, 1) == 11

    assert get_int_2d(a[::2, ::2], 0, 1) == 2
    assert get_int_2d(a[::2, ::2], 1, 0) == 8

    assert get_int_2d(a[1:, 1:], 0, 0) == 6
    set_int_2d(a[1:, 1:], 0, 0, 7)
    assert get_int_2d(a[1:, 1:], 0, 0) == 7

def test_fixedsize():
    from pymdspan import get_int_2d_fixed2n, get_int_2d_fixed22

    a = numpy.arange(16).reshape((4, 4))
    with pytest.raises(TypeError):
        get_int_2d_fixed22(a, 0, 0)

    assert get_int_2d_fixed22(a[:2, :2], 0, 0) == 0
    assert get_int_2d_fixed22(a[:2, :2], 0, 1) == 1
    assert get_int_2d_fixed22(a[:2, :2], 1, 0) == 4
    assert get_int_2d_fixed22(a[:2, :2], 1, 1) == 5

    assert get_int_2d_fixed22(a[::2, ::2], 0, 0) == 0
    assert get_int_2d_fixed22(a[::2, ::2], 1, 1) == 10
    assert get_int_2d_fixed22(a[::2, ::2], 0, 1) == 2
    assert get_int_2d_fixed22(a[::2, ::2], 1, 0) == 8

    a = numpy.arange(16).reshape((4, 4))
    with pytest.raises(TypeError):
        get_int_2d_fixed2n(a, 0, 0)
    with pytest.raises(TypeError):
        get_int_2d_fixed2n(a[:, :2], 0, 0)

    assert get_int_2d_fixed2n(a[:2, :2], 0, 0) == 0
    assert get_int_2d_fixed2n(a[:2, :2], 0, 1) == 1
    assert get_int_2d_fixed2n(a[:2, :2], 1, 0) == 4
    assert get_int_2d_fixed2n(a[:2, :2], 1, 1) == 5

    assert get_int_2d_fixed2n(a[:2, :], 0, 0) == 0
    assert get_int_2d_fixed2n(a[:2, :], 0, 1) == 1
    assert get_int_2d_fixed2n(a[:2, :], 1, 0) == 4
    assert get_int_2d_fixed2n(a[:2, :], 1, 1) == 5

    assert get_int_2d_fixed2n(a[::2, ::2], 0, 0) == 0
    assert get_int_2d_fixed2n(a[::2, ::2], 1, 1) == 10
    assert get_int_2d_fixed2n(a[::2, ::2], 0, 1) == 2
    assert get_int_2d_fixed2n(a[::2, ::2], 1, 0) == 8

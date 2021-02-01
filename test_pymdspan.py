import numpy

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
    try:
        sum_int_3d_dense(a[:2,:2,:])
    except TypeError:
        pass
    else:
        assert False # Not acceptable

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
    try:
        sum_int_2d_dense(a[:2,:2])
    except TypeError:
        pass
    else:
        assert False

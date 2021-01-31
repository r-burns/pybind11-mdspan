import numpy
from pymdspan import *

a = numpy.array([1, 2, 3], dtype=numpy.float64)
test(a)

a = numpy.array([[1, 2, 3],
                 [4, 5, 6],
                 [7, 8, 9]], dtype=numpy.float64)
test2(a)

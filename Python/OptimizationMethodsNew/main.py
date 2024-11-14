import OnedimensionalMethods
import numpy as np
import MultidimensionalMethods
from MultidimensionalMethods import PenaltyFunction


def test_f(x: float) -> float:
    return x*(x - 1)


def test_nd(x: np.ndarray) -> float:
    return np.dot(x, (x - 1))


def one_dim_search_methods():
    print(f'bisect result      : {OnedimensionalMethods.bisect(test_f, -10.0, 10.0):>12.3f}\n')
    print(f'golden_ratio result: {OnedimensionalMethods.golden_ratio(test_f, -10.0, 10.0):>12.3f}\n')
    print(f'fibonacci result   : {OnedimensionalMethods.fibonacci(test_f, -10.0, 10.0, 1.5e-6):>12.3f}')


def multi_dim_search_methods():
    target = PenaltyFunction()
    target.target = test_nd

    lhs = np.array((0.0, 0.0, 0.0))
    rhs = np.array((1.0, 1.0, 1.0))
    print(f'bisect result      : {MultidimensionalMethods.bisect(target, lhs, rhs)}\n')
    print(f'golden_ratio result      : {MultidimensionalMethods.golden_ratio(target, lhs, rhs)}\n')
    print(f'fibonacci result      : {MultidimensionalMethods.fibonacci(target,  lhs, rhs)}\n')
    x_start = np.array((0.0, 0.0, 0.0))
    print(f'perCordDescend result      : {MultidimensionalMethods.perCordDescend(target, x_start)}\n')
    x_start = np.array((0.0, 0.0, 0.0))
    print(f'gradient_descent result      : {MultidimensionalMethods.gradient_descent(target, x_start)}\n')


if __name__ == "__main__":
    # one_dim_search_methods()
    multi_dim_search_methods()

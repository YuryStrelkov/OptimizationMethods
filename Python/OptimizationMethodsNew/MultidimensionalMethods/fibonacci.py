from typing import Callable
import numpy as np


def closest_fib_pair(value: float) -> tuple[float, float, int]:
    f_t, f_1, f_2 = 0.0, 1.0, 1.0
    iteration = 0
    while f_2 < value:
        iteration += 1
        f_t, f_1 = f_1, f_2
        f_2 += f_t
    return f_1, f_2, iteration


def back_shift_fib(f_1: float, f_2: float) -> tuple[float, float]:
    f_t = f_2 - f_1
    return f_1, f_t


def fibonacci(function: Callable[[np.ndarray], float], lhs: np.ndarray, rhs: np.ndarray,
              eps: float = 1e-6) -> np.ndarray:

    assert (lhs.ndim == 1 and "lhs.ndim != 1")
    assert (rhs.ndim == 1 and "rhs.ndim != 1")
    assert (rhs.size == lhs.size and "rhs.size != lhs.size")

    f_1, f_2, iteration = closest_fib_pair(np.linalg.norm(rhs - lhs) / eps)

    x_l = lhs + (f_2 - f_1) / f_2 * (rhs - lhs)
    x_r = lhs + f_1 / f_2 * (rhs - lhs)
    f_2, f_1 = back_shift_fib(f_1, f_2)
    fl = function(x_l)
    fr = function(x_r)

    for index in range(iteration):
        if fl > fr:
            lhs = x_l
            x_l = x_r
            fl = fr
            x_r = lhs + f_1 * (rhs - lhs) / f_2
            fr = function(x_r)
        else:
            rhs = x_r
            x_r = x_l
            fr = fl
            x_l = lhs + (f_2 - f_1) * (rhs - lhs)/f_2
            fl = function(x_l)
        f_2, f_1 = back_shift_fib(f_1, f_2)

    print(f"\tfibonacci::args range : {np.linalg.norm(rhs - lhs)}")
    print(f"\tfibonacci::func probes: {iteration + 2}")
    return (rhs + lhs) * 0.5

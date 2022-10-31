from common import func_1d, _phi, _run_in_test_mode, max_iters_1d, accuracy_1d
from typing import Union
import numpy as np


def bisect(function: func_1d, x_0: float, x_1: float,
           _eps: float = accuracy_1d, max_iters: int = max_iters_1d) -> float:

    x_c: float = 0.0

    if x_0 > x_1:
        x_0, x_1 = x_1, x_0

    cntr: int = 0

    for cntr in range(max_iters + 1):
        if abs(x_1 - x_0) < _eps:
            break
        x_c = (x_1 + x_0) * 0.5
        if function(x_c + _eps) > function(x_c - _eps):
            x_1 = x_c
            continue
        x_0 = x_c

    if _run_in_test_mode:
        print(f"bisect iterations number : {cntr}")

    return x_c


def golden_ratio(function: func_1d, x_0: float, x_1: float,
                 _eps: float = accuracy_1d, max_iters: int = max_iters_1d) -> float:
    x_c: float = 0.0

    if x_0 > x_1:
        x_0, x_1 = x_1, x_0

    a = x_0, b = x_1

    cntr: int = 0

    for cntr in range(max_iters + 1):
        if abs(x_1 - x_0) < _eps:
            break

        dx = b - a
        x_0 = b - dx / _phi
        x_1 = a + dx / _phi

        if function(x_c + _eps) > function(x_c - _eps):
            a = x_0
            continue
        b = x_1

    if _run_in_test_mode:
        print(f"golden ratio iterations number : {cntr}")

    return x_c


def fibonacci_numbers(index: int) -> np.ndarray:
    if index < 0:
        return np.array([0])
    if index == 0 or index == 1:
        return np.array([1])
    fibonacci_numbers = np.zeros((index,), dtype=int)
    fibonacci_numbers[0] = 1
    fibonacci_numbers[1] = 1
    for i in range(2, index):
        fibonacci_numbers[i] = fibonacci_numbers[i-1] + fibonacci_numbers[i-2]
    return fibonacci_numbers


def closest_fibonacci_number(value: Union[float, int]) -> int:
    f1 = 1
    if value <= f1:
        return f1
    f2 = 2
    if value <= f2:
        return f2
    f3 = 3
    if value <= f3:
        return f3
    cntr: int = 3
    while True:
        f1 = f2
        f2 = f3
        f3 = f1 + f2
        if f3 > value:
            return cntr
        cntr += 1


def fibonacci(function: func_1d, x0: float, x1: float, _eps: float = accuracy_1d) -> float:
    if x0 > x1:
        x0, x1 = x1, x0

    a = x0, b = x1

    max_iters = closest_fibonacci_number((b - a) / _eps)

    f_n_s = fibonacci_numbers(max_iters)

    cntr: int = max_iters - 1

    while cntr != 2:
        if abs(x1 - x0) < _eps:
            break
        dx = b - a
        x0 = a + dx * f_n_s[cntr - 2] / f_n_s[cntr]
        x1 = a + dx * f_n_s[cntr - 1] / f_n_s[cntr]
        if function(x0) < function(x1):
            b = x1
            continue
        a = x0

    if _run_in_test_mode:
        print(f"fibonacci iterations number : {cntr}")

    return (x1 + x0) * 0.5




from typing import Callable
from MultidimensionalMethods import fibonacci
import numpy as np


def perCordDescend(function: Callable[[np.ndarray], float], x_start: np.ndarray, eps: float = 1e-6,
                   n_iters: int = 1000) -> np.ndarray:
    assert (x_start.ndim == 1 and "x_start.ndim != 1")
    x_0 = x_start.copy()
    x_1 = x_start.copy()
    step = np.ones(x_start.ndim)
    x_i, y_1, y_0 = np.zeros_like(x_start), np.zeros_like(x_start), np.zeros_like(x_start)
    opt_coord_n, coord_id = 0, 0
    probes = 0
    for i in range(n_iters):
        coord_id = i % x_0.size
        x_1[coord_id] -= eps
        y_0 = function(x_1)
        x_1[coord_id] += 2.0 * eps
        y_1 = function(x_1)
        x_1[coord_id] -= eps
        x_1[coord_id] += y_0 > y_1 if step else -step
        x_i = x_0[coord_id]
        x_1 = fibonacci(function, x_0, x_1, eps)
        x_0 = x_1.copy()
        if np.absolute(x_1[coord_id] - x_i) < 2 * eps:
            opt_coord_n += 1
            if opt_coord_n == x_1.size:
                print(f"\tperCordDescent::args range : {np.absolute(x_1[coord_id] - x_i)}")
                print(f"\tperCordDescent::func probes: {i}")
                return x_0
            continue
        opt_coord_n = 0

    print(f"\tperCordDescent::args range : {np.absolute(x_1[coord_id] - x_i)}")
    print(f"\tperCordDescent::func probes: {n_iters}")
    return x_0

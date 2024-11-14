from typing import Callable
import numpy as np

PSI = 0.6180339887498948
PHI = 1.6180339887498948


def golden_ratio(function: Callable[[np.ndarray], float], lhs: np.ndarray, rhs: np.ndarray,
                 eps: float = 1e-6, n_iters: int = 1000) -> np.ndarray:

    assert (lhs.ndim == 1 and "lhs.ndim != 1")
    assert (rhs.ndim == 1 and "rhs.ndim != 1")
    assert (rhs.size == lhs.size and "rhs.size != lhs.size")
    x_l, x_r = rhs - (rhs - lhs) * PSI, lhs + (rhs - lhs) * PSI
    fl, fr = function(x_l), function(x_r)
    iteration = 0
    for iteration in range(n_iters):
        if np.linalg.norm(rhs - lhs) <= 2 * eps:
            break
        if fl > fr:
            lhs = x_l
            x_l = x_r
            fl = fr
            x_r = lhs + (rhs - lhs) * PSI
            fr = function(x_r)
        else:
            rhs = x_r
            x_r = x_l
            fr = fl
            x_l = rhs - (rhs - lhs) * PSI
            fl = function(x_l)

    print(f"\tgolden_ratio::args range : {np.linalg.norm(rhs - lhs)}")
    print(f"\tgolden_ratio::func probes: {iteration + 2}")
    return (rhs + lhs) * 0.5

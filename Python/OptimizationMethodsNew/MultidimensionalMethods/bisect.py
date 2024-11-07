from typing import Callable
import numpy as np


def bisect(function: Callable[[np.ndarray], float], lhs: np.ndarray, rhs: np.ndarray,
           eps: float = 1e-6, n_iters: int = 1000) -> np.ndarray:
    assert (lhs.ndim == 1 and "lhs.ndim != 1")
    assert (rhs.ndim == 1 and "rhs.ndim != 1")
    assert (rhs.size == lhs.size and "rhs.size != lhs.size")
    direction = rhs - lhs
    direction = eps * direction / np.linalg.norm(direction)
    iteration = 0
    for iteration in range(n_iters):
        if np.linalg.norm(rhs - lhs) <= 2 * eps:
            break
        xc = (rhs + lhs) * 0.5
        if function(xc - direction) > function(xc + direction):
            lhs = xc
        else:
            rhs = xc
    print(f"\tbisect::args range : {np.linalg.norm(rhs - lhs)}")
    print(f"\tbisect::func probes: {iteration * 2}")
    return (rhs + lhs) * 0.5

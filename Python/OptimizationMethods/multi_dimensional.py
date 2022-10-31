from vectors_utils import direction, magnitude, gradient
from common import func_nd, accuracy_nd, max_iters_nd
import numpy as np


def bisect_nd(f: func_nd, x_0: np.ndarray, x_1: np.ndarray,
              accuracy: float = accuracy_nd, max_iters: int = max_iters_nd):
    if max_iters < 10:
        max_iters = 10
    x_0_: np.ndarray = x_0.copy()
    x_1_: np.ndarray = x_1.copy()
    x_c_: np.ndarray  # = (x_0_ + x_1_) * 0.5
    dir_: np.ndarray = direction(x_1_ - x_0_) * accuracy
    for i in range(max_iters):
        if magnitude(x_1_ - x_0_) < accuracy:
            break
        x_c_ = (x_1_ + x_0_) * 0.5
        if f(x_c_ + dir_) > f(x_c_ - dir_):
            x_1_ = x_c_
            continue
        x_0_ = x_c_
    return x_c_


def gradient_descend(func: func_nd, start_x: np.ndarray,
                     rate: float = 1.0, accuracy: float = accuracy_nd, max_iters: int = max_iters_nd) -> np.ndarray:
    if max_iters < 10:
        max_iters = 10

    x_i = start_x.copy()
    x_i_1: np.ndarray
    grad: np.ndarray
    cntr: int = 0
    while True:
        cntr += 1
        if cntr == max_iters:
            break
        grad = rate * gradient(func, x_i, accuracy)
        x_i_1 = x_i - grad
        x_i_1 = bisect_nd(func, x_i, x_i_1, accuracy, max_iters)
        if magnitude(x_i_1 - x_i) < accuracy:
            x_i = x_i_1
            break
        x_i = x_i_1

    return (x_i_1 + x_i) * 0.5

from typing import Callable
import numpy as np

_phi = 1.61803398874989484820

_run_in_test_mode: bool = False

func_1d = Callable[[float], float]

func_nd = Callable[[np.ndarray], float]

max_iters_1d = 1000

max_iters_nd = 1000

accuracy_1d = 1e-6

accuracy_nd = 1e-6

from vectors_utils import partial2
from common import func_nd
import numpy as np


def hessian(func: func_nd, args: np.ndarray, accuracy: float = 1e-6) -> np.ndarray:
    ddf = np.zeros((args.size, args.size,), dtype=float)
    for row in range(args.size):
        for col in range(row):
            ddf[row, col] = partial2(func, args, row, col, accuracy)
            ddf[col, row] = ddf[row, col]
    return ddf

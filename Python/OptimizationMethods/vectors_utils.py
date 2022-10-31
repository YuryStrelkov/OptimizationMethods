from common import func_nd
import numpy as np


def partial(func: func_nd, args: np.ndarray, arg_id: int, accuracy=1e-6) -> float:
    if args.size() <= arg_id:
        raise Exception("Partial derivative index out of bounds!")
    args[arg_id] += accuracy
    f_r = func(args)
    args[arg_id] -= 2.0 * accuracy
    f_l = func(args)
    args[arg_id] += accuracy
    return (f_r - f_l) / accuracy * 0.5


def partial2(func: func_nd, args: np.ndarray,
             arg_id_1: int, arg_id_2: int, accuracy=1e-6) -> float:
    if args.size() <= arg_id_2:
        raise Exception("Partial derivative index out of bounds!")
    args[arg_id_2] -= accuracy
    f_l = partial(func, args, arg_id_1, accuracy)
    args[arg_id_2] += 2.0 * accuracy
    f_r = partial(func, args, arg_id_1, accuracy)
    args[arg_id_2] -= accuracy
    return (f_r - f_l) / accuracy * 0.5


def magnitude(x: np.ndarray) -> float:
    return np.sqrt((x * x).sum())


def normalize(x: np.ndarray) -> np.ndarray:
    return x / magnitude(x)


def direction(x: np.ndarray) -> np.ndarray:
    return x / magnitude(x)


def gradient(func: func_nd, args: np.ndarray, accuracy: float = 1e-6) -> np.ndarray:
    df = np.zeros_like(args)
    for i in range(args.size):
        df[i] = partial(func, args, i, accuracy)
    return df

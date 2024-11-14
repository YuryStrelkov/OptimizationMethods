from typing import Callable


def bisect(function: Callable[[float], float], lhs: float, rhs: float,
           eps: float = 1e-6, n_iters: int = 1000) -> float:

    rhs, lhs = (lhs, rhs) if rhs < lhs else (rhs, lhs)
    iteration = 0
    for iteration in range(n_iters):
        if rhs - lhs <= 2 * eps:
            break
        xc = (rhs + lhs) * 0.5
        if function(xc - eps) > function(xc + eps):
            lhs = xc
        else:
            rhs = xc
    print(f"\tbisect::args range : {rhs - lhs}")
    print(f"\tbisect::func probes: {iteration * 2}")
    return (rhs + lhs) * 0.5

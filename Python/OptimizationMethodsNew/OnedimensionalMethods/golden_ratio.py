from typing import Callable

PSI = 0.6180339887498948
PHI = 1.6180339887498948


def golden_ratio(function: Callable[[float], float], lhs: float, rhs: float,
                 eps: float = 1e-6, n_iters: int = 1000) -> float:

    rhs, lhs = (lhs, rhs) if rhs < lhs else (rhs, lhs)
    x_l = rhs - (rhs - lhs)*PSI
    x_r = lhs + (rhs - lhs)*PSI
    fl = function(x_l)
    fr = function(x_r)

    iteration = 0
    for iteration in range(n_iters):
        if rhs - lhs <= 2 * eps:
            break
        if fl > fr:
            lhs = x_l
            x_l = x_r
            fl = fr
            x_r = lhs + (rhs - lhs)*PSI
            fr = function(x_r)
        else:
            rhs = x_r
            x_r = x_l
            fr = fl
            x_l = rhs - (rhs - lhs)*PSI
            fl = function(x_l)

    print(f"\tgolden_ratio::args range : {rhs - lhs}")
    print(f"\tgolden_ratio::func probes: {iteration + 2}")
    return (rhs + lhs) * 0.5

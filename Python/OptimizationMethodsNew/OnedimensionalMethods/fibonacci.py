from typing import Callable


def fibonacci(function: Callable[[float], float], lhs: float, rhs: float,
              eps: float = 1e-6) -> float:

    f_t, f_1, f_2 = 0.0, 1.0, 1.0
    rhs, lhs = (lhs, rhs) if rhs < lhs else (rhs, lhs)

    iteration = 0
    value = (rhs - lhs) / eps
    while f_2 < value:
        iteration += 1
        f_t, f_1 = f_1, f_2
        f_2 += f_t

    x_l = lhs + (f_2 - f_1) / f_2 * (rhs - lhs)
    x_r = lhs + f_1 / f_2 * (rhs - lhs)
    fl = function(x_l)
    fr = function(x_r)

    f_t = f_2 - f_1
    f_2, f_1 = f_1, f_t

    for index in range(iteration):
        if fl > fr:
            lhs = x_l
            x_l = x_r
            fl = fr
            x_r = lhs + f_1 * (rhs - lhs) / f_2
            fr = function(x_r)
        else:
            rhs = x_r
            x_r = x_l
            fr = fl
            x_l = lhs + (f_2 - f_1) * (rhs - lhs)/f_2
            fl = function(x_l)
        f_t = f_2 - f_1
        f_2, f_1 = f_1, f_t

    print(f"\tfibonacci::args range : {rhs - lhs}")
    print(f"\tfibonacci::func probes: {iteration + 2}")
    return (rhs + lhs) * 0.5

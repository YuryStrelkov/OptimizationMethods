from typing import Callable
import numpy as np


class PenaltyFunction:
    def __init__(self, target_function: Callable[[np.ndarray], float] = None):
        self._target = target_function
        self._boundaries = set()

    @property
    def target(self) -> Callable[[np.ndarray], float]:
        return self._target

    @target.setter
    def target(self, value: Callable[[np.ndarray], float]) -> None:
        assert isinstance(value, Callable)
        self._target = value

    def add_bound(self, bound: Callable[[np.ndarray], float]) -> bool:
        if bound in self._boundaries or not isinstance(bound, Callable):
            return False
        self._boundaries.add(bound)
        return True

    def del_bound(self, bound: Callable[[np.ndarray], float]) -> bool:
        if bound in self._boundaries:
            return False
        self._boundaries.remove(bound)
        return True

    def clear_boundaries(self):
        self._boundaries.clear()

    def __call__(self, arg: np.ndarray) -> float:
        return (self.target(arg) if self.target else 0.0) + sum(fn(arg) for fn in self._boundaries)

from typing import List, Tuple
from fractions import Fraction
import json

import numpy as np

SOLVE_MAX = 0
SOLVE_MIN = 1

LESS_EQUAL = 0
EQUAL = 1
MORE_EQUAL = 2


class Simplex:
    def __init__(self, c, b_mat, b, i=None):
        self._ineq = np.array([LESS_EQUAL for _ in range(b_mat.shape[0])])if i is None else i
        self._b_mat = b_mat
        self._bounds = b
        self._weights = c
        # [(переменные в базисе, с-м таблица)]
        self._s_tables: List[List[np.ndarray, np.ndarray]] = []
        self._curr_sim_t = np.copy(self._b_mat)
        self._basis_args = None # список индексов базисных аргументов
        self._f_mod_args = None # список индексов аргументов, которые модифицируют целевую функцию
        self._build_sm_table()

    @property
    def n_bounds(self) -> int:
        return self._bounds.size

    def _create_col(self, ineq_row: int, ineq: int):
        """
        Создаёт новую колонку в см таблице в соотвествии с типом ограничения
        :param ineq_row: строка с ограничением
        :param ineq: тип неравенство
        :return:
        """
        # last_table = self._s_tables[-1]
        if ineq == LESS_EQUAL:
            col = [[1.0] if i == ineq_row else [0.0] for i in range(self.n_bounds)]
            self._curr_sim_t = np.hstack((self._curr_sim_t, np.array(col)))
            return self._curr_sim_t.shape[1] - 1, - 1

        if ineq == EQUAL:
            col = [[1.0] if i == ineq_row else [0.0] for i in range(self.n_bounds)]
            self._curr_sim_t = np.append(self._curr_sim_t, np.array(col), axis=1)
            return self._curr_sim_t.shape[1] - 1, self._curr_sim_t.shape[1] - 1

        if ineq == MORE_EQUAL:
            col_1 = [[-1.0] if i == ineq_row else [0.0] for i in range(self.n_bounds)]
            col_2 = [[1.0] if i == ineq_row else [0.0] for i in range(self.n_bounds)]
            self._curr_sim_t = np.vstack((self._curr_sim_t, np.array(col_1)))
            self._curr_sim_t = np.vstack((self._curr_sim_t, np.array(col_2)))
            return self._curr_sim_t.shape[1] - 2, self._curr_sim_t.shape[1] - 1

        raise RuntimeError("incorrect inequality parameter!")

    def _build_sm_table(self):
        # TODO Добавить проверку на отрицательность элементов вектора B
        self._basis_args = np.linspace(0, self.n_bounds - 1, self.n_bounds, dtype=int)
        for ineq_id, ineq in enumerate(self._ineq.flat):
            basis_arg_id, basis_arg_id_add = self._create_col(ineq_id, ineq)
            if basis_arg_id != -1:
                self._basis_args[ineq_id] = basis_arg_id
        # сиплекс разности (вектор  -С)
        rows, cols = self._curr_sim_t.shape
        self._curr_sim_t = np.vstack((self._curr_sim_t, np.zeros((1, cols), dtype=float)))

        for index, value in enumerate(self._weights.flat):
            self._curr_sim_t[rows, index] = -value
        # сиплекс ограничения (вектор B)
        self._curr_sim_t = np.hstack((self._curr_sim_t, np.zeros((rows + 1, 1), dtype=float)))

        for index, value in enumerate(self._bounds.flat):
            self._curr_sim_t[index, cols] = value

    def simplex_table(self) -> str:
        def _xi(i):
            return f"x{i}"

        def _as_frac(value: float):
            f = Fraction(value).limit_denominator(50)
            i, d, n = f.numerator//f.denominator, f.denominator, f.numerator
            if n == 0:
                return f"{n}"
            if i == 0:
                return f"{n}/{d}"
            if i == n * d:
                return f"{i}"
            return f"{i} {n - d * i}/{d}"
        rows, cols = self._curr_sim_t.shape
        header = f" {(cols * 14 + 2) * '_'} \n"
        header += '|\t\t\t |' + '|'.join(f'{_xi(i):^12}' for i in range(cols-1)) + f"|{'b':^12}|\n"
        for arg_id, row in zip(self._basis_args.flat, self._curr_sim_t):
            header += f"|{_xi(arg_id):^12}|" + '|'.join(f'{_as_frac(rowi):^12}' for rowi in row)
            header += f"|\n"

        sim_d = self._curr_sim_t[-1]
        header += f"|{'dC':^12}|" + '|'.join(f'{_as_frac(v):^12}' for v in sim_d) + f"|\n"
        header += f" {(cols * 14 + 2) * '_'} \n"
        return header


def read_simplex(path_to_file: str):
    def _read_array(node):
        if 'shape' not in node:
            raise RuntimeError('shape is not defined')
        shape = tuple(map(int, node['shape']))
        if 'data' not in node:
            raise RuntimeError('data is not defined')
        array = np.array(list(map(float, node['data'])), dtype=np.float32)
        return array.reshape(shape)

    with open(path_to_file, 'rt') as input_file:
        raw_data = json.loads(input_file.read())
        solve_type = int(raw_data['solve_type']) if 'solve_type' in raw_data else SOLVE_MAX

        if 'weights' not in raw_data:
            raise RuntimeError('weights is not defined')
        weights = _read_array(raw_data['weights'])

        if 'bounds' not in raw_data:
            raise RuntimeError('bounds is not defined')
        bounds = _read_array(raw_data['bounds'])

        if 'bounds_matrix' not in raw_data:
            raise RuntimeError('bounds_matrix is not defined')
        bounds_matrix = _read_array(raw_data['bounds_matrix'])

        inequalities = np.array(list(map(int, raw_data['inequalities'])), dtype=int) if 'inequalities' in raw_data\
            else np.array([LESS_EQUAL for _ in range(bounds_matrix.shape[0])], dtype=int)
        return Simplex(weights, bounds_matrix, bounds, inequalities)
        # print(solve_type)
        # print(weights)
        # print(bounds)
        # print(bounds_matrix)
        # print(inequalities)


simplex = read_simplex('sm_task.json')
print(simplex.simplex_table())
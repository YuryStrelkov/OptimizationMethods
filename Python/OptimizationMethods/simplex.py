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
    def __init__(self, weights: np.ndarray, bounds_mat: np.ndarray, bounds: np.ndarray, i=None):
        flag, message = Simplex._validate(weights, bounds_mat, bounds)
        if not flag:
            raise RuntimeError(message)
        self._ineq = np.array([LESS_EQUAL for _ in range(bounds_mat.shape[0])])if i is None else i
        self._b_mat = bounds_mat
        self._bounds = bounds
        self._weights = weights
        # [(переменные в базисе, с-м таблица)]
        self._s_tables: List[List[np.ndarray, np.ndarray]] = []
        self._curr_sim_t = np.copy(self._b_mat)
        self._basis_args = None  # список индексов базисных аргументов
        self._f_mod_args = None  # список индексов аргументов, которые модифицируют целевую функцию
        self._build_sm_table()

    @staticmethod
    def _validate(weights: np.ndarray, bounds_mat: np.ndarray, bounds: np.ndarray) -> Tuple[bool, str]:
        _n_weights = weights.size
        _n_bounds  = bounds.size
        _m_rows, _m_cols = bounds_mat.shape

        if _n_weights != _m_cols:
            return False, "Количество столбцов матрицы ограничений не совпадает с количеством весов..."

        if _n_bounds != _m_rows:
            return False, "Количество строк матрицы ограничений не совпадает с количеством ограничений..."

        return True, ""

    @property
    def n_bounds(self) -> int:
        return self._bounds.size

    def _create_col(self, ineq_row: int, ineq: int):
        """
        Создаёт новую колонку в см таблице в соответствии с типом ограничения.
        :param ineq_row: строка с ограничением.
        :param ineq: тип неравенство.
        :return:
        """
        # last_table = self._s_tables[-1]
        if ineq == LESS_EQUAL:
            col = [[1.0] if i == ineq_row else [0.0] for i in range(self.n_bounds)]
            self._curr_sim_t = np.hstack((self._curr_sim_t, np.array(col)))
            return self._curr_sim_t.shape[1] - 1, - 1

        if ineq == EQUAL:
            col = [[1.0] if i == ineq_row else [0.0] for i in range(self.n_bounds)]
            self._curr_sim_t = np.hstack((self._curr_sim_t, np.array(col)))
            return self._curr_sim_t.shape[1] - 1, self._curr_sim_t.shape[1] - 1

        if ineq == MORE_EQUAL:
            col_1 = [[-1.0] if i == ineq_row else [0.0] for i in range(self.n_bounds)]
            col_2 = [[1.0] if i == ineq_row else [0.0] for i in range(self.n_bounds)]
            self._curr_sim_t = np.hstack((self._curr_sim_t, np.array(col_1)))
            self._curr_sim_t = np.hstack((self._curr_sim_t, np.array(col_2)))
            return self._curr_sim_t.shape[1] - 2, self._curr_sim_t.shape[1] - 1

        raise RuntimeError("incorrect inequality parameter!")

    def _build_sm_table(self):
        """
        Составляет симплекс таблицу на основе прочитанных данных.
        :return:
        """
        # TODO Добавить проверку на отрицательность элементов вектора B
        self._basis_args = np.linspace(0, self.n_bounds - 1, self.n_bounds, dtype=int)
        for ineq_id, ineq in enumerate(self._ineq.flat):
            basis_arg_id, basis_arg_id_add = self._create_col(ineq_id, ineq)
            if basis_arg_id != -1:
                self._basis_args[ineq_id] = basis_arg_id

        # текущий размер симплекс таблицы
        rows, cols = self._curr_sim_t.shape

        # симплекс разности (вектор  -С)
        self._curr_sim_t = np.vstack((self._curr_sim_t, np.zeros((1, cols), dtype=float)))

        for index, value in enumerate(self._weights.flat):
            self._curr_sim_t[rows, index] = -value

        # симплекс ограничения (вектор B)
        self._curr_sim_t = np.hstack((self._curr_sim_t, np.zeros((rows + 1, 1), dtype=float)))

        for index, value in enumerate(self._bounds.flat):
            self._curr_sim_t[index, cols] = value

    def _get_main_col(self) -> int:
        last_row = self._curr_sim_t[-1, :-1]
        col_id = -1
        delta = 0.0
        for i, v in enumerate(last_row.flat):
            if v >= delta:
                continue
            delta = v
            col_id = i
        return col_id

    def _get_main_row(self, col_id: int) -> int:
        n_rows, n_cols = self._curr_sim_t.shape
        n_rows -= 1  # строки симплекс разностей не учитываются, потому и -1
        delta = 1e32
        main_row_id = -1
        for row_id in range(n_rows):
            a_ik = self._curr_sim_t[row_id, col_id]
            delta_curr = self._curr_sim_t[row_id, col_id] / a_ik
            if a_ik < 0:
                continue
            if delta_curr > delta:
                continue
            delta = delta_curr
            main_row_id = row_id

        return main_row_id

    def solve(self):
        ...

    def simplex_table(self) -> str:
        """
        Создаёт строковое представление симплекс таблицы.
        :return: строковое представление симплекс таблицы, лол.
        """
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
        header  = f" {'_' * (12 * (cols + 1) + cols )}\n"
        header += f"|{'_'*12}|" + '|'.join(f'{_xi(i):_^12}' for i in range(cols-1)) + f"|{'b':_^12}|\n"
        for arg_id, row in zip(self._basis_args.flat, self._curr_sim_t):
            header += f"|{_xi(arg_id):^12}|" + '|'.join(f'{_as_frac(row_i):^12}' for row_i in row)
            header += f"|\n"

        sim_d = self._curr_sim_t[-1]
        header += f"|{'dC':^12}|" + '|'.join(f'{_as_frac(v):^12}' for v in sim_d) + f"|\n"
        header += f" {'-' * (12 * (cols + 1) + cols )}\n"
        return header


def read_simplex(path_to_file: str) -> List[Simplex]:
    def _read_array(node):
        if 'shape' not in node:
            raise RuntimeError('shape is not defined')
        shape = tuple(map(int, node['shape']))
        if 'data' not in node:
            raise RuntimeError('data is not defined')
        array = np.array(list(map(float, node['data'])), dtype=np.float32)
        return array.reshape(shape)

    def _read_simplex(json_node) -> Simplex:
        solve_type = int(json_node['solve_type']) if 'solve_type' in json_node else SOLVE_MAX

        if 'weights' not in json_node:
            raise RuntimeError('weights is not defined')
        weights = _read_array(json_node['weights'])

        if 'bounds' not in json_node:
            raise RuntimeError('bounds is not defined')
        bounds = _read_array(json_node['bounds'])

        if 'bounds_matrix' not in json_node:
            raise RuntimeError('bounds_matrix is not defined')
        bounds_matrix = _read_array(json_node['bounds_matrix'])

        inequalities = np.array(list(map(int, json_node['inequalities'])), dtype=int) if 'inequalities' in json_node \
            else np.array([LESS_EQUAL for _ in range(bounds_matrix.shape[0])], dtype=int)
        return Simplex(weights, bounds_matrix, bounds, inequalities)

    with open(path_to_file, 'rt') as input_file:
        raw_data = json.loads(input_file.read())
        if "problems" in raw_data:
            return [_read_simplex(node) for node in raw_data['problems']]
        return [_read_simplex(raw_data)]


simplexes = read_simplex('Resources/sm_task.json')

[print(simplex.simplex_table()) for simplex in simplexes]

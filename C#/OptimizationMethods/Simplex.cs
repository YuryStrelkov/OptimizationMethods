using System;
using System.Collections.Generic;
using System.Text;

namespace OptimizationMethods
{
    public enum Sign
    {
        Equal = 0,
        Less = 1,
        More = 2
    }

    public enum SimplexProblemType
    {
        Min = 0,
        Max = 1,
    }

    public class Simplex
    {
        ////////////////////
        /// Lab. work #5 ///
        ////////////////////
        /// <summary>
        /// список знаков в неравенств в системе ограничений
        /// </summary>
        private List<Sign> _inequalities;

        /// <summary>
        /// список индексов переменных которые войдут в целевую функию, модифицируя ее
        /// </summary>
        private List<int> _fModArgs;

        /// <summary>
        ///индексы естественных переменных
        /// </summary>
        private List<int> _naturalArgsIds;

        /// <summary>
        /// список индексов текущих базисных переменных 
        /// </summary>
        private List<int> _basisArgs;

        /// <summary>
        /// Симплекс таблица
        /// </summary>
        private Matrix _simplexTable;

        /// <summary>
        /// матрица ограничений
        /// </summary>
        private Matrix _boundsMatrix;

        /// <summary>
        /// вектор ограничений
        /// </summary>
        private Vector _boundsVector;

        /// <summary>
        /// вектор стоимостей
        /// </summary>
        private Vector _pricesVector;

        /// <summary>
        /// режим поиска решения
        /// </summary>
        private SimplexProblemType mode = SimplexProblemType.Max;

        public bool IsTargetFuncModified()
        {
            return _fModArgs.Count != 0;
        }

        /// <summary>
        /// Проверяет оптимальность текущего опорного плана. Исследуется положительность 
        /// симплекс-разностей в последней строке СТ в диапазоне от 1:n-1.
        /// Если целевая функция была модифицирована, то исследует две последних строки.
        /// Если среди элементов от 1:n-1 в последней строке нет отрицательных, то проверяет 
        /// на неотрицательность только те элементы предпоследней строки, которые не являются
        /// искусственными.
        /// </summary>
        /// <param name="A">СМ таблицa</param>
        /// <param name="mode"></param>
        /// <returns></returns>
        public bool IsPlanOptimal()
        {
            /// <summary>
            /// Проверяем значения последней строки сиплекс-разностей
            /// на положительность. Если все положительны, то план оптимален.
            /// </summary>

            Vector row = _simplexTable[_simplexTable.NRows - 1];

            bool opt = true;

            foreach (double v in row)
            {
                if (v >= 0) continue;
                opt = false;
                break;
            }

            /// <summary>
            /// если мы модифицировали целевую функцию, то среди списка естественнхых
            /// агументов проверям на положительнность предпослднюю строку симплекс-разностей 
            /// </summary>

            if (IsTargetFuncModified())
            {
                if (!opt)
                {
                    return opt;
                }
                row = _simplexTable[_simplexTable.NRows - 2];

                foreach (int id in _naturalArgsIds)
                {
                    if (row[id] >= 0) continue;
                    opt = false;
                    break;
                }
            }

            return opt;
        }
        /// <summary>
        /// Определяет ведущий столбец. Среди элементов строки симплекс-разностей ищет максимальны по модулю 
        /// отрицательный элемент. Если целевая функция была модифицирована и среди последней строки нет отрицательных
        /// элементов, то посик таковых будет продолжен среди только тех элементов предпоследней строки, которые не
        /// являются искусственными.
        /// </summary>
        /// <param name="A"></param>
        /// <returns></returns>
        private int GetMainCol()
        {

            Vector row = _simplexTable[_simplexTable.NRows - 1];

            double delta = 0;

            int index = -1;

            for (int i = 0; i < row.Count - 1; i++)
            {
                if (row[i] >= delta)
                {
                    continue;
                }
                delta = row[i];
                index = i;
            }

            if (IsTargetFuncModified() && index == -1)
            {
                Vector row_add = _simplexTable[_simplexTable.NRows - 2];

                foreach (int id in _naturalArgsIds)
                {
                    if (row_add[id] >= delta)
                    {
                        continue;
                    }
                    delta = row_add[id];
                    index = id;
                }
            }
            return index;
        }

        /// <summary>
        /// Определяет ведущую строку 
        /// </summary>
        /// <param name="simplex_col">ведущий столбец</param>
        /// <param name="A">СМ таблица</param>
        /// <returns></returns>
        int GetMainRow(int simplex_col)
        {

            double delta = 1e12;

            int index = -1;

            double a_ik;

            int b_index = _simplexTable[0].Count - 1;

            int rows_n = IsTargetFuncModified() ? _simplexTable.NRows - 2 : _simplexTable.NRows - 1;

            for (int i = 0; i < rows_n; i++)
            {
                a_ik = _simplexTable[i][simplex_col];

                if (a_ik < 0) continue;
                if (_simplexTable[i][b_index] / a_ik > delta) continue;
                delta = _simplexTable[i][b_index] / a_ik;
                index = i;
            }

            return index;
        }
     
        /// <summary>
        /// строит виртуальный базисный вектор
        /// </summary>
        /// <param name="ineq_id"></param>
        /// <param name="_ineq"></param>
        /// <param name="col_index"></param>
        /// <param name="col_index_aditional"></param>
        private void BuildVirtualBasisCol(int ineq_id, Sign _ineq, ref int col_index, ref int col_index_aditional)
        {
            if (_ineq == Sign.Equal)
            {
                for (int row = 0; row < _simplexTable.NRows; row++)
                {
                    if (row == ineq_id)
                    {
                        _simplexTable[row].PushBack(1.0);
                        continue;
                    }
                    _simplexTable[row].PushBack(0.0);
                }

                col_index = _simplexTable[0].Count - 1;

                col_index_aditional = _simplexTable[0].Count - 1;

                return ;
            }

            if (_ineq == Sign.More)
            {
                for (int row = 0; row < _simplexTable.NRows; row++)
                {
                    if (row == ineq_id)
                    {
                        _simplexTable[row].PushBack(-1.0);

                        _simplexTable[row].PushBack(1.0);

                        continue;
                    }

                    _simplexTable[row].PushBack(0.0);

                    _simplexTable[row].PushBack(0.0);
                }

                col_index = _simplexTable[0].Count - 2;

                col_index_aditional = _simplexTable[0].Count - 1;

                return ;
            }

            for (int row = 0; row < _simplexTable.NRows; row++)
            {
                if (row == ineq_id)
                {
                    _simplexTable[row].PushBack(1.0);
                    continue;
                }
                _simplexTable[row].PushBack(0.0);
            }
            
            col_index = _simplexTable[0].Count - 1;
            
            col_index_aditional = -1;

            return;
        }

        /// <summary>
        /// Строит СМ таблицу для задачи вида:
        /// Маирица системы ограниченй:
        ///		|u 0 0|	
        /// A = |0 v 0|
        ///		|0 0 w|
        /// Вектор ограничений
        ///		|a|	
        /// b = |d|
        ///		|f|
        /// с -коэффициенты целевой функции 
        /// f = (x,c)->extr
        ///	|u 0 0|   |x| <= |b|
        /// |0 v 0| * |x| >= |f|
        ///	|0 0 w|   |x| =  |d|
        /// 
        ///  СМ таблицу из A,b,c параметров
        /// </summary>
        /// <param name="A"> Ax <= b   -> (A|I)(x|w) = b </param>
        /// <param name="c"> (c,x) ->((-c|0),(x|w)) </param>
        /// <param name="ineq"> знак неравентсва =, >=, <= </param>
        /// <param name="b"></param>
        ///( A|I)  b
        ///(-c|0)  F(x,c)
        private void BuildSimplexTable()
        {
            _simplexTable = new Matrix(_boundsMatrix);
            _naturalArgsIds.Clear();
            _basisArgs.Clear();
            _fModArgs.Clear();
            ///
            /// Если среди вектора b есть отрицательные значения, то соответствующие строки
            /// матрицы ограничений умножаем на мину один и меняем знак сравнения
            ///
            for (int row = 0; row < _simplexTable.NRows; row++)
            {
                if (_boundsVector[row] >= 0)
                {
                    continue;
                }

                _inequalities[row] = _inequalities[row] == Sign.Less ? Sign.More : Sign.Less;

                _boundsVector[row] *= -1;

                _simplexTable[row] = _simplexTable[row] * (-1.0);
            }


            for (int i = 0; i < _pricesVector.Count; i++)
            {
                _naturalArgsIds.Add(i);
            }
            /// <summary>
            /// построение искуственного базиса
            /// </summary>
            int basis_arg_id = -1;
            int basis_arg_id_add = -1;
            for (int ineq_id = 0; ineq_id < _inequalities.Count; ineq_id++)
            {
                BuildVirtualBasisCol(ineq_id, _inequalities[ineq_id], ref basis_arg_id, ref basis_arg_id_add);

                _naturalArgsIds.Add(basis_arg_id);

                if (basis_arg_id_add != -1)
                {
                    _basisArgs.Add(basis_arg_id_add);
                    _fModArgs.Add(basis_arg_id_add);
                    continue;
                }

                _basisArgs.Add(basis_arg_id);
            }

            /// <summary>
            /// добавим столбец ограницений
            /// </summary>

            for (int row = 0; row < _simplexTable.NRows; row++) _simplexTable[row].PushBack(_boundsVector[row]);

            /// <summary>
            /// Построение симплекс разностей
            /// </summary>

            Vector s_deltas = new Vector(_simplexTable.NCols);

            if (mode == SimplexProblemType.Max)
            {
                for (int j = 0; j < _simplexTable.NCols; j++) s_deltas[j] = j < _pricesVector.Count ? -_pricesVector[j] : 0.0;
            }
            else
            {
                for (int j = 0; j < _simplexTable.NCols; j++) s_deltas[j] = j < _pricesVector.Count ? _pricesVector[j] : 0.0;
            }

            _simplexTable.AddRow(s_deltas);

            /// <summary>
            /// Если целевая функуция не была модифицирована
            /// </summary>

            if (!IsTargetFuncModified())
            {
                return;
            }

            /// <summary>
            /// Если всё же была...
            /// </summary>
            Vector s_deltas_add = new Vector(_simplexTable.NCols);
            for (int j = 0; j < _simplexTable.NCols; j++)  s_deltas_add[j] = 0.0; //
            foreach (int fModArgsId in _fModArgs) s_deltas_add[fModArgsId] = 1.0;

            _simplexTable.AddRow(s_deltas_add);
        }

        private bool ExcludeModArgs()
        {
            if (!IsTargetFuncModified())
            {
                return false;
            }

            int last_row_id = _simplexTable.NRows - 1;

            for (int i = 0; i < _fModArgs.Count; i++)
            {
                for (int row = 0; row < _simplexTable.NRows; row++)
                {
                    if (_simplexTable[row][_fModArgs[i]] == 0) continue;

                    double arg = _simplexTable[last_row_id][_fModArgs[i]] / _simplexTable[row][_fModArgs[i]];

                    _simplexTable[last_row_id] = _simplexTable[last_row_id] - arg * _simplexTable[row];

                    break;
                }
            }

            return true;
        }

        private bool ValidateSolution()
        {

            double val = 0;

            int n_rows = IsTargetFuncModified() ? _simplexTable.NRows - 2 : _simplexTable.NRows - 1;

            int n_cols = _simplexTable.NCols - 1;

            for (int i = 0; i < _basisArgs.Count; i++)
            {
                if (_basisArgs[i] < NaturalArgsN()) val += _simplexTable[i][n_cols] * _pricesVector[_basisArgs[i]];
            }
            if (mode == SimplexProblemType.Max)
            {
                if (Math.Abs(val - _simplexTable[n_rows][n_cols]) < 1e-5)
                {
                    if (!IsTargetFuncModified()) return true;
                    return true & (Math.Abs(_simplexTable[_simplexTable.NRows - 1][_simplexTable.NCols - 1]) < 1e-5);
                }
            }
            if (Math.Abs(val + _simplexTable[n_rows][n_cols]) < 1e-5)
            {
                if (!IsTargetFuncModified()) return true;
                return true & (Math.Abs(_simplexTable[_simplexTable.NRows - 1][_simplexTable.NCols - 1]) < 1e-5);
            }
            return false;
        }

        public int NaturalArgsN()
        {
            return _pricesVector.Count;
        }

        public Matrix BoundsMatrix()
        {
            return _boundsMatrix;
        }

        public Vector BoundsCoeffs()
        {
            return _boundsVector;
        }

        public Vector PricesCoeffs()
        {
            return _pricesVector;
        }

        public List<Sign> Inequations()
        {
            return _inequalities;
        }

        public List<int> BasisArgsuments()
        {
            return _basisArgs;
        }

        public Matrix SimplexTable()
        {
            return _simplexTable;
        }

        public Vector CurrentSimplexSolution(bool only_natural_args = false)
        {
            int count = only_natural_args ? NaturalArgsN() : _simplexTable.NCols - 1;
            
            Vector solution = new Vector(count);

            for (int i = 0; i < _basisArgs.Count; i++)
            {
                if (_basisArgs[i] >= count) continue;

                solution[_basisArgs[i]] = _simplexTable[i][_simplexTable.NCols - 1];
            }
            return solution;
        }
        public string SimplexToString()//Matrix table, List<int> basis)
        {
            if (_simplexTable.NRows == 0) return "";

            StringBuilder sb = new StringBuilder();

            int i = 0;

            sb.AppendFormat("{0,-6}", " ");

            for (; i < _simplexTable.NCols - 1; i++)
            {
                sb.AppendFormat("|{0,-12}", " x " + (i + 1).ToString());
            }
            sb.AppendFormat("|{0,-12}", " b");

            sb.Append("\n");

            int n_row = -1;

            foreach (Vector row in _simplexTable.Rows)
            {
                n_row++;

                if (IsTargetFuncModified())
                {
                    if (n_row == _simplexTable.NRows - 2)
                    {
                        sb.AppendFormat("{0,-6}", " d0");
                    }
                    else if (n_row == _simplexTable.NRows - 1)
                    {
                        sb.AppendFormat("{0,-6}", " d1");
                    }
                    else 
                    {
                        sb.AppendFormat("{0,-6}", " x " + (_basisArgs[n_row] + 1).ToString());
                    }
                }
                else 
                {
                    if (n_row == _simplexTable.NRows - 1)
                    {
                        sb.AppendFormat("{0,-6}", " d");
                    }
                    else
                    {
                        sb.AppendFormat("{0,-6}", " x " + (_basisArgs[n_row] + 1).ToString());
                    }
                }

                for (int col = 0; col < row.Count; col++)
                {
                    if (row[col] >= 0)
                    {
                        sb.AppendFormat("|{0,-12}", " " + NumericUtils.ToRationalStr(row[col]));
                        continue;
                    }
                    sb.AppendFormat("|{0,-12}", NumericUtils.ToRationalStr(row[col]));

                }
                sb.Append("\n");
            }
            sb.Append("\n");

            return sb.ToString();
        }

        public Vector Solve(SimplexProblemType mode = SimplexProblemType.Max)
        {
            this.mode = mode;

            Console.WriteLine($"SimplexProblemType : {SimplexProblemType.Max.ToString()}\n");

            Vector solution = new Vector(NaturalArgsN());

            BuildSimplexTable();

            double a_ik;

            int main_row;

            int main_col;

            Console.WriteLine("Start simplex table:");
            Console.WriteLine(SimplexToString());

            if (ExcludeModArgs())
            {
                // второй этап, если задача должна решаться двух проходным(двух этапным) алгоритмом
                Console.WriteLine("Simplex table after args exclude:");
                Console.WriteLine(SimplexToString());
            }

            while (!IsPlanOptimal())
            {
                main_col = GetMainCol();

                if (main_col == -1) break;

                main_row = GetMainRow(main_col);

                if (main_row == -1)
                {
                    /// Невозможность определить ведущую строку свидейтельствует о том, что обрасть поиска неограничена
                    Console.WriteLine("Unable to get main row. Simplex is probably boundless...");
                    return null;
                }

                _basisArgs[main_row] = main_col;

                a_ik = _simplexTable[main_row][main_col];

                _simplexTable[main_row] = _simplexTable[main_row] * (1.0 / a_ik);

                for (int i = 0; i < _simplexTable.NRows; i++)
                {
                    if (i == main_row) continue;
                    _simplexTable[i] = _simplexTable[i] - _simplexTable[i][main_col] * _simplexTable[main_row];
                }
                solution = CurrentSimplexSolution();

#if DEBUG
                Console.WriteLine($"a_main {{{ main_row + 1}, {main_col + 1}}} = {NumericUtils.ToRationalStr(a_ik)}\n");
                Console.WriteLine(SimplexToString());
                Console.WriteLine($"current solution: {NumericUtils.ToRationalStr(solution)}\n");
#endif
            }
            if (ValidateSolution())
            {
                solution = CurrentSimplexSolution(true);
                /// формирование ответа
                Console.WriteLine($"solution: {NumericUtils.ToRationalStr(solution)}\n");
                return solution;
            }
            Console.WriteLine("Simplex is unresolvable");
            /// значение целевой функции не равно ее значению от найденного плана
            return null;
        }
        public Simplex(Matrix a, Vector c, List<Sign> ineq, Vector b)
        {
            if (b.Count != ineq.Count) throw new Exception("Error simplex creation :: b.size() != inequation.size()");
            if (a.NRows != ineq.Count) throw new Exception("Error simplex creation :: A.rows_number() != inequation.size()");
            if (a.NCols != c.Count) throw new Exception("Error simplex creation :: A.cols_number() != price_coeffs.size()");

            _naturalArgsIds = new List<int>();
            _basisArgs      = new List<int>();
            _fModArgs       = new List<int>();
            _boundsVector   = b;
            _boundsMatrix   = a;
            _pricesVector   = c;
            _inequalities   = ineq;
        }
        public Simplex(Matrix a, Vector c, Vector b)
        {
            if (b.Count != b.Count) throw new Exception("Error simplex creation :: b.size() != bouns_coeffs.size()");
            if (a.NCols != c.Count) throw new Exception("Error simplex creation :: A.cols_number() != price_coeffs.size()");

            _inequalities = new List<Sign>();
            for (int i = 0; i < b.Count; i++) _inequalities.Add(Sign.Less);
            _naturalArgsIds = new List<int>();
            _basisArgs      = new List<int>();
            _fModArgs       = new List<int>();
            _boundsVector   = b;
            _boundsMatrix   = a;
            _pricesVector = c;
        }
    }
}

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

    public enum SymplexProblemType
    {
        Min = 0,
        Max = 1,
    }

    public class Symplex
    {
        ////////////////////
        /// Lab. work #5 ///
        ////////////////////
        /// <summary>
        /// список знаков в неравенств в системе ограничений
        /// </summary>
        private List<Sign> ineqs;

        /// <summary>
        /// список индексов переменных которые войдут в целевую функию, модифицируя ее
        /// </summary>
        private List<int> f_mod_args;

        /// <summary>
        ///индексы естественных переменных
        /// </summary>
        private List<int> natural_args_ids;

        /// <summary>
        ///индексы переменных, не являющихся искусственными
        /// </summary>
        private List<int> artificial_args_ids;

        /// <summary>
        /// список индексов текущих базисных переменных 
        /// </summary>
        private List<int> basis_args;

        /// <summary>
        /// Симплекс таблица
        /// </summary>
        private Matrix symplex_t;

        /// <summary>
        /// матрица ограничений
        /// </summary>
        private Matrix bounds_m;

        /// <summary>
        /// вектор ограничений
        /// </summary>
        private Vector bounds_v;

        /// <summary>
        /// вектор стоимостей
        /// </summary>
        private Vector prices_v;

        /// <summary>
        /// режим поиска решения
        /// </summary>
        private SymplexProblemType mode = SymplexProblemType.Max;

        public bool IsTargetFuncModified()
        {
            return f_mod_args.Count != 0;
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

            Vector row = symplex_t[symplex_t.NRows - 1];

            bool opt = true;

            for (int i = 0; i < row.Count - 1; i++)
            {
                if (row[i] < 0)
                {
                    opt = false;
                    break;
                }
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
                Vector row_ = symplex_t[symplex_t.NRows - 2];

                foreach (int id in natural_args_ids)
                {
                    if (row_[id] < 0)
                    {
                        opt = false;
                        break;
                    }
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

            Vector row = symplex_t[symplex_t.NRows - 1];

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
                Vector row_add = symplex_t[symplex_t.NRows - 2];

                foreach (int id in natural_args_ids)
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
        /// <param name="symplex_col">ведущий столбец</param>
        /// <param name="A">СМ таблица</param>
        /// <returns></returns>
        int GetMainRow(int symplex_col)
        {

            double delta = 1e12;

            int index = -1;

            double a_ik;

            int b_index = symplex_t[0].Count - 1;

            int rows_n = IsTargetFuncModified() ? symplex_t.NRows - 2 : symplex_t.NRows - 1;

            for (int i = 0; i < rows_n; i++)
            {
                a_ik = symplex_t[i][symplex_col];

                if (a_ik < 0)
                {
                    continue;
                }
                if (symplex_t[i][b_index] / a_ik > delta)
                {
                    continue;
                }
                delta = symplex_t[i][b_index] / a_ik;
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
                for (int row = 0; row < symplex_t.NRows; row++)
                {
                    if (row == ineq_id)
                    {
                        symplex_t[row].PushBack(1.0);
                        continue;
                    }
                    symplex_t[row].PushBack(0.0);
                }

                col_index = symplex_t[0].Count - 1;

                col_index_aditional = symplex_t[0].Count - 1;

                return ;
            }

            if (_ineq == Sign.More)
            {
                for (int row = 0; row < symplex_t.NRows; row++)
                {
                    if (row == ineq_id)
                    {
                        symplex_t[row].PushBack(-1.0);

                        symplex_t[row].PushBack(1.0);

                        continue;
                    }

                    symplex_t[row].PushBack(0.0);

                    symplex_t[row].PushBack(0.0);
                }

                col_index = symplex_t[0].Count - 2;

                col_index_aditional = symplex_t[0].Count - 1;

                return ;
            }

            for (int row = 0; row < symplex_t.NRows; row++)
            {
                if (row == ineq_id)
                {
                    symplex_t[row].PushBack(1.0);
                    continue;
                }
                symplex_t[row].PushBack(0.0);
            }
            
            col_index = symplex_t[0].Count - 1;
            
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
        private void BuildSymplexTable()
        {
            symplex_t = new Matrix(bounds_m);
            natural_args_ids.Clear();
            basis_args.Clear();
            f_mod_args.Clear();
            artificial_args_ids.Clear();
            ///
            /// Если среди вектора b есть отрицательные значения, то соответствующие строки
            /// матрицы ограничений умножаем на мину один и меняем знак сравнения
            ///
            for (int row = 0; row < symplex_t.NRows; row++)
            {
                if (bounds_v[row] >= 0)
                {
                    continue;
                }

                ineqs[row] = ineqs[row] == Sign.Less ? Sign.More : Sign.Less;

                bounds_v[row] *= -1;

                symplex_t[row] = symplex_t[row] * (-1.0);
            }


            for (int i = 0; i < prices_v.Count; i++)
            {
                natural_args_ids.Add(i);
            }
            /// <summary>
            /// построение искуственного базиса
            /// </summary>
            int basis_arg_id = -1;
            int basis_arg_id_add = -1;
            for (int ineq_id = 0; ineq_id < ineqs.Count; ineq_id++)
            {
                BuildVirtualBasisCol(ineq_id, ineqs[ineq_id], ref basis_arg_id, ref basis_arg_id_add);

                natural_args_ids.Add(basis_arg_id);

                if (basis_arg_id_add != -1)
                {
                    basis_args.Add(basis_arg_id_add);
                    f_mod_args.Add(basis_arg_id_add);
                    artificial_args_ids.Add(basis_arg_id_add);
                    continue;
                }

                basis_args.Add(basis_arg_id);
            }

            /// <summary>
            /// добавим столбец ограницений
            /// </summary>

            for (int row = 0; row < symplex_t.NRows; row++)
            {
                symplex_t[row].PushBack(bounds_v[row]);
            }

            /// <summary>
            /// Построение симплекс разностей
            /// </summary>

            Vector s_deltas = new Vector(symplex_t.NCols);

            if (mode == SymplexProblemType.Max)
            {
                for (int j = 0; j < s_deltas.Count; j++)
                {
                    s_deltas[j] = j < prices_v.Count ? -prices_v[j] : 0.0;
                }
            }
            else
            {
                for (int j = 0; j < s_deltas.Count; j++)
                {
                    s_deltas[j] = j < prices_v.Count ? prices_v[j] : 0.0;
                }
            }

            symplex_t.AddRow(s_deltas);

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
            Vector s_deltas_add = new Vector(symplex_t.NCols);

            for (int j = 0; j < f_mod_args.Count; j++)
            {
                s_deltas_add[f_mod_args[j]] = 1.0;
            }

            symplex_t.AddRow(s_deltas_add);
        }

        private bool ExcludeModArgs()
        {
            if (!IsTargetFuncModified())
            {
                return false;
            }

            int last_row_id = symplex_t.NRows - 1;

            for (int i = 0; i < f_mod_args.Count; i++)
            {
                for (int row = 0; row < symplex_t.NRows; row++)
                {
                    if (symplex_t[row][f_mod_args[i]] != 0)
                    {
                        double arg = symplex_t[last_row_id][f_mod_args[i]] / symplex_t[row][f_mod_args[i]];

                        symplex_t[last_row_id] = symplex_t[last_row_id] - arg * symplex_t[row];

                        break;
                    }
                }
            }

            return true;
        }

        private bool ValidateSolution()
        {

            double val = 0;

            int n_rows = IsTargetFuncModified() ? symplex_t.NRows - 2 : symplex_t.NRows - 1;

            int n_cols = symplex_t.NCols - 1;

            for (int i = 0; i < basis_args.Count; i++)
            {
                if (basis_args[i] < NaturalArgsN())
                {
                    val += symplex_t[i][n_cols] * prices_v[basis_args[i]];
                }
            }
            if (mode == SymplexProblemType.Max)
            {
                if (Math.Abs(val - symplex_t[n_rows][n_cols]) < 1e-5)
                {
                    if (IsTargetFuncModified())
                    {
                        return true & (Math.Abs(symplex_t[symplex_t.NRows - 1][symplex_t.NCols - 1]) < 1e-5);
                    }

                    return true;
                }
            }
            if (Math.Abs(val + symplex_t[n_rows][n_cols]) < 1e-5)
            {
                if (IsTargetFuncModified())
                {
                    return true & (Math.Abs(symplex_t[symplex_t.NRows - 1][symplex_t.NCols - 1]) < 1e-5);
                }

                return true;
            }
            return false;
        }

        public int NaturalArgsN()
        {
            return prices_v.Count;
        }

        public Matrix BoundsMatrix()
        {
            return bounds_m;
        }

        public Vector BoundsCoeffs()
        {
            return bounds_v;
        }

        public Vector PricesCoeffs()
        {
            return prices_v;
        }

        public List<Sign> Inequations()
        {
            return ineqs;
        }

        public List<int> BasisArgsuments()
        {
            return basis_args;
        }

        public Matrix SymplexTable()
        {
            return symplex_t;
        }

        public Vector CurrentSymplexSolution(bool only_natural_args = false)
        {
            Vector solution = new Vector(only_natural_args ? NaturalArgsN() : symplex_t.NCols - 1);

            for (int i = 0; i < basis_args.Count; i++)
            {
                if (basis_args[i] >= solution.Count)
                {
                    continue;
                }

                solution[basis_args[i]] = symplex_t[i][symplex_t.NCols - 1];
            }
            return solution;
        }
        public string SymplexToString()//Matrix table, List<int> basis)
        {
            if (symplex_t.NRows == 0)
            {
                return "";
            }

            StringBuilder sb = new StringBuilder();

            int i = 0;

            sb.AppendFormat("{0,-6}", " ");

            for (; i < symplex_t.NCols - 1; i++)
            {
                sb.AppendFormat("|{0,-12}", " x " + (i + 1).ToString());
            }
            sb.AppendFormat("|{0,-12}", " b");

            sb.Append("\n");

            int n_row = -1;

            foreach (Vector row in symplex_t.Rows)
            {
                n_row++;

                if (IsTargetFuncModified())
                {
                    if (n_row == symplex_t.NRows - 2)
                    {
                        sb.AppendFormat("{0,-6}", " d0");
                    }
                    else if (n_row == symplex_t.NRows - 1)
                    {
                        sb.AppendFormat("{0,-6}", " d1");
                    }
                    else 
                    {
                        sb.AppendFormat("{0,-6}", " x " + (basis_args[n_row] + 1).ToString());
                    }
                }
                else 
                {
                    if (n_row == symplex_t.NRows - 1)
                    {
                        sb.AppendFormat("{0,-6}", " d");
                    }
                    else
                    {
                        sb.AppendFormat("{0,-6}", " x " + (basis_args[n_row] + 1).ToString());
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

        public Vector Solve(SymplexProblemType mode = SymplexProblemType.Max)
        {
            this.mode = mode;

            Console.WriteLine($"SymplexProblemType : {SymplexProblemType.Max.ToString()}\n");


            Vector solution = new Vector(NaturalArgsN());

            SolutionType system_condition = Matrix.CheckSystem(bounds_m, bounds_v);

            if (system_condition == SolutionType.None)
            {
                return solution;
            }

            if (system_condition == SolutionType.Single)
            {
                return Matrix.Linsolve(bounds_m, bounds_v);
            }

            BuildSymplexTable();

            double a_ik;

            int main_row;

            int main_col;

            Console.WriteLine("Start symplex table:");
            Console.WriteLine(SymplexToString());

            if (ExcludeModArgs())
            {
                // второй этап, если задача должна решаться двух проходным(двух этапным) алгоритмом
                Console.WriteLine("Symplex table after args exclude:");
                Console.WriteLine(SymplexToString());
            }

            while (!IsPlanOptimal())
            {
                main_col = GetMainCol();

                if (main_col == -1)
                {
                    break;
                }

                main_row = GetMainRow(main_col);

                if (main_row == -1)
                {
                    /// Невозможность определить ведущую строку свидейтельствует о том, что обрасть поиска неограничена
                    Console.WriteLine("Unable to get main row. Symplex is probably boundless...");
                    return null;
                }

                basis_args[main_row] = main_col;

                a_ik = symplex_t[main_row][main_col];

                symplex_t[main_row] = symplex_t[main_row] * (1.0 / a_ik);

                for (int i = 0; i < symplex_t.NRows; i++)
                {
                    if (i == main_row)
                    {
                        continue;
                    }
                    symplex_t[i] = symplex_t[i] - symplex_t[i][main_col] * symplex_t[main_row];
                }
                solution = CurrentSymplexSolution();

#if DEBUG
                Console.WriteLine($"a_main {{{ main_row + 1}, {main_col + 1}}} = {NumericUtils.ToRationalStr(a_ik)}\n");
                Console.WriteLine(SymplexToString());
                Console.WriteLine($"current solution: {NumericUtils.ToRationalStr(solution)}\n");
#endif
            }
            if (ValidateSolution())
            {
                solution = CurrentSymplexSolution(true);
                /// формирование ответа
                Console.WriteLine($"solution: {NumericUtils.ToRationalStr(solution)}\n");
                return solution;
            }
            Console.WriteLine("Symplex is unresolvable");
            /// значение целевой функции не равно ее значению от найденного плана
            return null;
        }
        public Symplex(Matrix a, Vector c, List<Sign> _ineq, Vector b)
        {
            if (b.Count != _ineq.Count)
            {
                throw new Exception("Error symplex creation :: b.size() != inequation.size()");
            }

            if (a.NRows != _ineq.Count)
            {
                throw new Exception("Error symplex creation :: A.rows_number() != inequation.size()");
            }

            if (a.NCols != c.Count)
            {
                throw new Exception("Error symplex creation :: A.cols_number() != price_coeffs.size()");
            }

            natural_args_ids    = new List<int>();
            basis_args          = new List<int>();
            f_mod_args          = new List<int>();
            artificial_args_ids = new List<int>();

            bounds_v = b;

            bounds_m = a;

            prices_v = c;

            ineqs    = _ineq;
        }
        public Symplex(Matrix a, Vector c, Vector b)
        {
            if (b.Count != b.Count)
            {
                throw new Exception("Error symplex creation :: b.size() != bouns_coeffs.size()");
            }

            if (a.NCols != c.Count)
            {
                throw new Exception("Error symplex creation :: A.cols_number() != price_coeffs.size()");
            }

            ineqs = new List<Sign>();

            for (int i = 0; i < b.Count; i++)
            {
                ineqs.Add(Sign.Less);
            }

            natural_args_ids = new List<int>();
            basis_args = new List<int>();
            f_mod_args = new List<int>();
            artificial_args_ids = new List<int>();

            bounds_v = b;

            bounds_m = a;

            prices_v = c;
        }
    }
}

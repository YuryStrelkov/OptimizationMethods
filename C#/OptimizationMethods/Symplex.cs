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
    public enum SolutionType
    {
        Single = 0,
        Infinite = 1,
        None = 2
    }

    public enum SymplexProblemType
    {
        Min = 0,
        Max = 1,
    }

    public static class Symplex
    {
        ////////////////////
        /// Lab. work #5 ///
        ////////////////////
        private static string ToRationalStr(double value, bool fullRational = true)
        {
            int r_part;
            
            int num;
            
            int denom;

            NumericUtils.DecimalToRational(value,out r_part, out num, out denom);
            if (num == 0)
            {
                return r_part.ToString();
            }
            if (r_part == 0)
            {
                return num.ToString() + "/" + denom.ToString();
            }

            if (fullRational)
            {
                return ((num + Math.Abs(r_part) * denom) * (r_part >= 0 ? 1 : -1)).ToString() + "/" + denom.ToString();
            }
            return denom.ToString() + " " + num.ToString() + "/" + denom.ToString();
        }
        private static string ToRationalStr(Vector value, bool fullRational = true)
        {
            string str = "{ ";
            for (int i = 0; i < value.Size - 1; i++)
            {
                str += ToRationalStr(value[i], fullRational);
                str += ", ";
            }
            str += ToRationalStr(value[value.Size - 1], fullRational);

            str += " }";
            return str;
        }

        public static string SymplexToString(Matrix table, List<int> basis)
        {
            if (table.NRows == 0)
            {
                return "";
            }

            StringBuilder sb = new StringBuilder();

            int i = 0;

            sb.AppendFormat("{0,-6}", " ");

            for (; i < table.NCols - 1; i++)
            {
                sb.AppendFormat("|{0,-12}", " x " + (i + 1).ToString());
            }
            sb.AppendFormat("|{0,-12}", " b");

            sb.Append("\n");

            int n_row = -1;

            foreach (Vector row in table.Rows)
            {
                n_row++;

                if (n_row == table.NRows - 1)
                {
                    sb.AppendFormat("{0,-6}", " d");
                }
                else
                {
                    sb.AppendFormat("{0,-6}", " x " + (basis[n_row] + 1).ToString());
                }

                for (int col = 0; col < row.Size; col++)
                {
                    if (row[col] >= 0)
                    {
                        sb.AppendFormat("|{0,-12}", " " + ToRationalStr(row[col]));
                        continue;
                    }
                    sb.AppendFormat("|{0,-12}", ToRationalStr(row[col]));

                }
                sb.Append("\n");
            }
            sb.Append("\n");

            return sb.ToString();
        }
        /// <summary>
        /// Проверяет совместность СЛАУ вида Ax = b. Используется теорема Кронекера-Капелли 
        /// </summary>
        /// <param name="A"></param>
        /// <param name="b"></param>
        /// <returns>0 - нет решений, 1 - одно решение, 2 - бесконечное множествое решений</returns>
        public static SolutionType CheckSystem(Matrix A, Vector b)
        {
            Matrix a = new Matrix(A);

            int rank_a = Matrix.Rank(a);

            Matrix ab = new Matrix(A);

            int rank_a_b = Matrix.Rank(ab.AddCol(b));

#if DEBUG
            Console.WriteLine($"rank ( A ) {rank_a}\n");
            Console.WriteLine($"rank (A|b) {rank_a_b}\n");
            if (rank_a == rank_a_b)
            {
                Console.WriteLine("one solution\n");
            }
            if (rank_a < rank_a_b)
            {
                Console.WriteLine("infinite amount of solutions\n");
            }
            if (rank_a > rank_a_b)
            {
                Console.WriteLine("no solutions\n");
            }
#endif

            if (rank_a == rank_a_b)
            {
                return SolutionType.Single;
            }
            if (rank_a < rank_a_b)
            {
                return SolutionType.Infinite;
            }
            if (rank_a > rank_a_b)
            {
                return SolutionType.None;
            }
            throw new Exception("error :: check_system");
        }
        /// <summary>
        /// Проверяет оптимальность плана в соответсвии с тем типом экстремума, которыей требуется найти.
        /// Провеяются элменты от 1:n-1 полсдней строки СМ таблицы 
        /// </summary>
        /// <param name="A">СМ таблицa</param>
        /// <param name="mode"></param>
        /// <returns></returns>
        public static bool IsPlanOptimal(Matrix A, SymplexProblemType mode)
        {
            Vector deltas = A[A.NRows - 1];

            if (mode == SymplexProblemType.Max)
            {
                for (int i = 0; i < deltas.Size - 1; i++)
                {
                    if (deltas[i] < 0)
                    {
                        return false;
                    }
                }
                return true;
            }

            for (int i = 0; i < deltas.Size - 1; i++)
            {
                if (deltas[i] > 0)
                {
                    return false;
                }
            }
            return true;
        }
        /// <summary>
        /// Определяет ведущий столбец в соответсвии с тем типом экстремума, который требуется найти.
        /// Исследуются элменты от 1:n-1 полсдней строки СМ таблицы 
        /// </summary>
        /// <param name="A"></param>
        /// <returns></returns>
        public static int GetMainCol(Matrix A, SymplexProblemType mode)
        {
            double delta = 0;

            int index = -1;

            Vector c = A[A.NRows - 1];

            if (SymplexProblemType.Min == mode)
            {
                for (int i = 0; i < c.Size - 1; i++)
                {
                    if (c[i] <= 0)
                    {
                        continue;
                    }
                    if (c[i] > delta)
                    {
                        continue;
                    }

                    delta = c[i];

                    index = i;
                }
                return index;
            }

            for (int i = 0; i < c.Size - 1; i++)
            {
                if (c[i] >= 0)
                {
                    continue;
                }
                if (Math.Abs(c[i]) < delta)
                {
                    continue;
                }

                delta = Math.Abs(c[i]);

                index = i;
            }

            return index;
        }
        /// <summary>
        /// Определяет ведущую строку 
        /// </summary>
        /// <param name="symplex_col">ведущий столбец</param>
        /// <param name="A">СМ таблица</param>
        /// <returns></returns>
        /// <summary>

        public static int GetMainRow(int symplex_col, Matrix A)
        {
            double delta = 1e12;

            int index = -1;

            double a_ik;

            int b_index = A[0].Size - 1;

            for (int i = 0; i < A.NRows; i++)
            {
                a_ik = A[i][symplex_col];

                if (a_ik < 0)
                {
                    continue;
                }
                if (A[i][b_index] / a_ik > delta)
                {
                    continue;
                }
                delta = A[i][b_index] / a_ik;
                index = i;
            }
            return index;
        }
        /// Выводит текущее решение СМ таблицы для не искусственных переменных
        /// </summary>
        /// <param name="A">СМ таблица</param>
        /// <param name="basis">список базисных параметров</param>
        /// <param name="n_agrs">количество исходных переменных</param>
        /// <returns></returns>
        public static Vector CurrentSymplexSolution(Matrix A, List<int> basis, int n_agrs)
        {

            Vector solution = new Vector(n_agrs);
            for (int i = 0; i < basis.Count; i++)
            {
                if (basis[i] >= n_agrs)
                {
                    continue;
                }
                solution[basis[i]] = A[i][A[0].Size - 1];
            }
            return solution;
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
        public static List<int> BuildSymplexTable(out Matrix table, Matrix A, Vector c, Vector b, List<Sign> ineq)
        {
            if (A.NRows != b.Size)
            {
                throw new Exception("Incorrect symplex problem");
            }
            if (A.NCols != c.Size)
            {
                throw new Exception("Incorrect symplex problem");
            }

            table = new Matrix(A);

            List<int> basis = new List<int>();

            int cntr = 0;

            Vector row;

            for (int n_row = 0; n_row < A.Rows.Count; n_row++)
            {
                row = table.Rows[n_row];

                basis.Add(-1);

                for (int j = 0; j < b.Size; j++)
                {
                    if (ineq[j] == Sign.Equal)
                    {
                        basis[basis.Count - 1] = j;
                        continue;
                    }
                    if (ineq[j] == Sign.More)
                    {
                        if (cntr == j)
                        {
                            basis[basis.Count - 1] = c.Size + j;
                            row.PushBack(-1.0);
                            continue;
                        }
                        row.PushBack(0.0);
                        continue;
                    }
                    if (ineq[j] == Sign.Less)
                    {

                        if (cntr == j)
                        {
                            basis[basis.Count - 1] = c.Size + j;
                            row.PushBack(1.0);
                            continue;
                        }
                        row.PushBack(0.0);
                        continue;
                    }
                }
                row.PushBack(b[cntr]);

                if (b[cntr] < 0)
                {
                    for (int i = 0; i < row.Size; i++)
                    {
                        row[i] *= (-1);
                    }
                }
                cntr++;
            }

            Vector C = new Vector(table.NCols);

            for (int j = 0; j < C.Size; j++)
            {
                C[j] = j < c.Size ? -c[j] : 0.0;
            }

            table.AddRow(C);

            return basis;

        }

        public static Vector SymplexSolve(Matrix a, Vector c, Vector b, List<Sign> ineq, SymplexProblemType mode = SymplexProblemType.Max)
        {
            var system_condition = CheckSystem(a, b);

            if (system_condition == SolutionType.None)
            {
                return null;
            }

            if (system_condition == SolutionType.Single)
            {
                return Matrix.Linsolve(a, b);
            }

            double a_ik;

            int main_row;

            int main_col;

            Matrix A;

            List<int> basis = BuildSymplexTable(out A, a, c, b, ineq);
#if DEBUG
            Console.WriteLine($"Start symplex table:\n\n{SymplexToString(A, basis)}");
#endif

            while (!IsPlanOptimal(A, mode))
            {
                main_col = GetMainCol(A, mode);

                if (main_col == -1)
                {
                    break;
                }

                main_row = GetMainRow(main_col, A);

                if (main_row == -1)
                {
                    break;
                }

                basis[main_row] = main_col;

                a_ik = A[main_row][main_col];

                A[main_row] = A[main_row] * (1.0 / a_ik);

                for (int i = 0; i < A.NRows; i++)
                {
                    if (i == main_row)
                    {
                        continue;
                    }
                    A[i] = A[i] - A[i][main_col] * A[main_row];
                }
#if DEBUG
                Console.WriteLine($"a_main {{{(main_row + 1)} , {(main_col + 1)}}} = {ToRationalStr(a_ik)} \n");
                Console.WriteLine(SymplexToString(A, basis));
                Console.WriteLine($"current_solution : {ToRationalStr(CurrentSymplexSolution(A, basis, c.Size))}\n");
#endif
            }
            /// формирование ответа
            return CurrentSymplexSolution(A, basis, c.Size);
        }

    }
}

using System.Collections.Generic;
using System.Threading.Tasks;
using System;

namespace MathUtils
{
    public enum SolutionType
    {
        Single = 0,
        Infinite = 1,
        None = 2
    }
    public sealed class DoubleMatrix: TemplateVector<DoubleVector>
    {
        /// <summary>
        /// Массив строк матрицы
        /// </summary>
        public DoubleMatrix AddCol(DoubleVector col)
        {
            if (col.Count != NRows) throw new Exception("Error::AddCol::col.Size != NRows");
            for (int i = 0; i < Count; i++) this[i].PushBack(col[i]);
            return this;
        }

        public DoubleMatrix AddRow(DoubleVector row)
        {
            if (row.Count != NCols) throw new Exception("Error::AddRow::row.Size != NCols");
            PushBack(row);
            return this;
        }

        public new DoubleMatrix PushBack(DoubleVector row) 
        {
            if (NRows == 0) 
            {
                base.PushBack(row);
                return this;
            }
            if (row.Count != NCols) throw new Exception("Error::AddRow::row.Size != NCols");
            base.PushBack(row);
            return this;
        }

        public override string ToString() => $"{{\n  {string.Join(",\n  ", this.Map(v => v.ToString()))}\n}}";

        /// <summary>
        ///  Количество строк
        /// </summary>
        public int NRows => Count;

        /// <summary>
        /// Количество столбцов
        /// </summary>
        public int NCols => NRows == 0 ? 0 : this[0].Count;

        public IEnumerable<DoubleVector> Rows => this;

        /// <summary>
        /// Рамерность матрицы
        /// </summary>
        public int[] Size => new int[] { NRows, NCols };

        /// <summary>
        /// Конструктор матрицы из массива строк
        /// </summary>
        /// <param name="rows"></param>
        public DoubleMatrix() : base() { }
        
        public DoubleMatrix(params DoubleVector[] rows): base()
        {
            if (rows == null) throw new Exception("Data is null...");

            if (rows.Length == 0) throw new Exception("Data is empty...");

            int maxSize = int.MinValue;
            
            int minSize = int.MaxValue;

            foreach (DoubleVector v in rows)
            {
                if (v.Count > maxSize) maxSize = v.Count;
                if (v.Count < minSize) minSize = v.Count;
            }
            
            if (maxSize != minSize) throw new Exception("Incorrect DoubleMatrix data");

            foreach (DoubleVector r in rows) PushBack(r);
        }

        /// <summary>
        /// Конструктор матрцы по ее размерам и элементу по умолчанию
        /// </summary>
        /// <param name="n_rows">колическтво строк</param>
        /// <param name="n_cols">количество столбцов</param>
        /// <param name="defualtVal">значение элементов матрицы по умолчанию</param>
        public DoubleMatrix(int n_rows, int n_cols) : base()
        {
            for (int i = 0; i < n_rows; i++) PushBack(new DoubleVector(n_cols));
        }
        
        /// <summary>
        /// Конструктор копирования
        /// </summary>
        /// <param name="original"></param>
        public DoubleMatrix(DoubleMatrix original) : base()
        {
            foreach (DoubleVector row in original) PushBack(new DoubleVector(row));
        }

        /// <summary>
        /// Конструктор через итератор
        /// </summary>
        /// <param name="original"></param>
        public DoubleMatrix(IEnumerable<DoubleVector> original) : base(original){}

        /// <summary>
        /// </summary>
        /// <param name="f"></param>
        /// <param name="x"></param>
        /// <param name="eps"></param>
        /// <returns></returns>
        public static DoubleMatrix Hessian(FunctionND f, DoubleVector x, double eps)
        {
            DoubleMatrix res = new DoubleMatrix(x.Count, x.Count);
            int row, col;
            for (row = 0; row < res.NRows; row++)
                for (col = 0; col <= row; col++)
                    res[row][col] = res[col][row] = DoubleVector.Partial2(f, x, row, col, eps);
            return res;
        }

        public static DoubleMatrix Hessian(FunctionND f, DoubleVector x) => Hessian(f, x, NumericCommon.NUMERIC_ACCURACY_MIDDLE);

        /// <summary>
        /// Определят ранг матрицы
        /// </summary>
        /// <param name="A"></param>
        /// <returns></returns>
        public static int Rank(DoubleMatrix DoubleMatrix)
        {
            DoubleMatrix A = new DoubleMatrix(DoubleMatrix);

            int n = A.NRows;

            int m = A.NCols;

            int rank = 0;

            bool[] row_selected = new bool[n];

            for (int i = 0; i < row_selected.Length; i++) row_selected[i] = false;

            for (int i = 0; i < m; i++)
            {
                int j;

                for (j = 0; j < n; j++) if (!row_selected[j] && Math.Abs(A[j][i]) > NumericCommon.NUMERIC_ACCURACY_HIGH) break;

                if (j != n)
                {
                    ++rank;

                    row_selected[j] = true;

                    for (int p = i + 1; p < m; p++) A[j][p] /= A[j][i];

                    for (int k = 0; k < n; k++)
                        if (k != j && Math.Abs(A[k][i]) > NumericCommon.NUMERIC_ACCURACY_HIGH)
                            for (int p = i + 1; p < m; p++) A[k][p] -= A[j][p] * A[k][i];
                }
            }
            return rank;
        }

        /// <summary>
        /// Создаёт матрицу из нулей
        /// </summary>
        /// <param name="n_rows">число строк</param>
        /// <param name="n_cols">число столбцов</param>
        /// <returns>матрица нулей</returns>
        public static DoubleMatrix Zeros(int n_rows, int n_cols) => new DoubleMatrix(n_rows, n_cols);

        /// <summary>
        /// Создаёт квадратную матрицу нулей
        /// </summary>
        /// <param name="size">сторона матрицы</param>
        /// <returns>квадратная матрицы нулей</returns>
        public static DoubleMatrix Zeros(int size) => Zeros(size, size);

        /// <summary>
        /// Создаёт единичную матрицу
        /// </summary>
        /// <param name="n_rows">число строк</param>
        /// <param name="n_cols">число столбцов</param>
        /// <returns>единичная матрица</returns>
        public static DoubleMatrix Identity(int n_rows, int n_cols)
        {
            DoubleMatrix I = new DoubleMatrix(n_rows, n_cols);
            for (int i = 0; i < Math.Min(n_rows, n_cols); i++) I[i][i] = 1.0;
            return I;
        }
        
        /// <summary>
        /// Создаёт квадратную единичную матрицу
        /// </summary>
        /// <param name="size">сторона матрицы</param>
        /// <returns>квадратная единичная матрица</returns>
        public static DoubleMatrix Identity(int size) => Identity(size, size);

        /// <summary>
        /// LU hазложение матрицы на нижнюю и верхнюю треугольные матрицы
        /// </summary>
        /// <param name="src">Матрица разложение которой нужно провести</param>
        /// <param name="low">Нижняя треугольная матрица</param>
        /// <param name="up">Верхняя треугольная матрица</param>
        public static void LU(ref DoubleMatrix src, out DoubleMatrix low, out DoubleMatrix up)
        {
            if (src.NCols != src.NRows) throw new Exception("LU decomposition error::non square DoubleMatrix");

            low = new DoubleMatrix(src.NCols, src.NRows);

            up = new DoubleMatrix(src.NCols, src.NRows);

            int i, j, k;

            for (i = 0; i < src.NRows; i++)
            {
                for (j = 0; j < src.NRows; j++)
                {
                    if (j >= i)
                    {
                        low[j][i] = src[j][i];

                        for (k = 0; k < i; k++) low[j][i] = low[j][i] - low[j][k] * up[k][i];
                    }
                }

                for (j = 0; j < src.NRows; j++)
                {
                    if (j < i) continue;
                    if (j == i)
                    {
                        up[i][j] = 1.0;
                        continue;
                    }
                    up[i][j] = src[i][j] / low[i][i];
                    for (k = 0; k < i; k++) up[i][j] = up[i][j] - ((low[i][k] * up[k][j]) / low[i][i]);
                }
            }
        }
        
        /// <summary>
        /// Вспомогательный метод рещения системы уравнений вида Ax = b при условии, что найдено разложение A = LU
        /// </summary>
        /// <param name="low">L</param>
        /// <param name="up">U</param>
        /// <param name="b">b</param>
        /// <returns>x</returns>
        private static DoubleVector Linsolve(ref DoubleMatrix low, ref DoubleMatrix up, ref DoubleVector b)
        {
            double det = 1.0;

            DoubleVector x, z;

            for (int i = 0; i < up.NRows; i++) det *= (up[i][i] * up[i][i]);

            if (Math.Abs(det) < NumericCommon.NUMERIC_ACCURACY_HIGH) return null;
            
            z = new DoubleVector(up.NRows);

            double tmp;

            for (int i = 0; i < z.Count; i++)
            {
                tmp = 0.0;

                for (int j = 0; j < i; j++) tmp += z[j] * low[i][j];
                
                z[i] = (b[i] - tmp) / low[i][i];
            }

            x = new DoubleVector(up.NRows);

            for (int i = z.Count - 1; i >= 0; i--)
            {
                tmp = 0.0;

                for (int j = i + 1; j < z.Count; j++) tmp += x[j] * up[i][j];
                
                x[i] = z[i] - tmp;
            }

            return x;
        }
        
        /// <summary>
        /// Решение системы уравнений вида Ax = b
        /// </summary>
        /// <param name="mat">A</param>
        /// <param name="b">b</param>
        /// <returns>x</returns>
        public static DoubleVector Linsolve(DoubleMatrix mat, DoubleVector b)
        {
            if (mat.NRows != mat.NCols) throw new Exception("non square DoubleMatrix");

            DoubleMatrix low, up;

            LU(ref mat, out low, out up);

            return Linsolve(ref low, ref up, ref b);
        }
        
        /// <summary>
        /// Рассчитывает обратную матрицу
        /// </summary>
        /// <param name="mat">матрица для которой ищем обратную</param>
        /// <returns>обратная матрица</returns>
        public static DoubleMatrix Invert(DoubleMatrix mat)
        {
            if (mat.NRows != mat.NCols) throw new Exception("non square DoubleMatrix");

            DoubleMatrix low, up, inv;

            LU(ref mat, out low, out up);

            DoubleVector b, col;

            b = new DoubleVector(mat.NRows);

            inv = Zeros(mat.NRows);

            for (int i = 0; i < mat.NCols; i++)
            {
                b[i] = 1.0;
                col = Linsolve(ref low, ref up, ref b);
                if (col == null)    throw new Exception("unable to find DoubleMatrix inversion");
                if (col.Count == 0) throw new Exception("unable to find DoubleMatrix inversion");
                b[i] = 0.0;
                for (int j = 0; j < mat.NRows; j++) inv[j][i] = col[j];
            }
            return inv;
        }
        
        /// <summary>
        /// Транспонирование матрицы
        /// </summary>
        /// <param name="mat"></param>
        /// <returns></returns>
        public static DoubleMatrix Transpose(DoubleMatrix mat)
        {
            DoubleMatrix trans = new DoubleMatrix(mat.NCols, mat.NRows);
            Parallel.For(0, mat.NRows, i => { for (int j = 0; j < mat.NCols; j++) trans[j][i] = mat[i][j]; });
            return trans;
        }

        /// <summary>
        /// Проверяет совместность СЛАУ вида Ax = b. Используется теорема Кронекера-Капелли 
        /// </summary>
        /// <param name="A"></param>
        /// <param name="b"></param>
        /// <returns>0 - нет решений, 1 - одно решение, 2 - бесконечное множествое решений</returns>
        public static SolutionType CheckSystem(DoubleMatrix A, DoubleVector b)
        {
            int rank_a = Rank(A);

            DoubleMatrix ab = new DoubleMatrix(A);

            int rank_a_b = Rank(ab.AddCol(b));

#if DEBUG
            Console.WriteLine($"rank ( A ) {rank_a}\n");
            Console.WriteLine($"rank (A|b) {rank_a_b}\n");
            if (rank_a == rank_a_b) Console.WriteLine("one solution\n");
            if (rank_a  < rank_a_b) Console.WriteLine("infinite amount of solutions\n");
            if (rank_a  > rank_a_b) Console.WriteLine("no solutions\n");
#endif

            if (rank_a == rank_a_b) return SolutionType.Single;
            if (rank_a  < rank_a_b) return SolutionType.Infinite;
            if (rank_a  > rank_a_b) return SolutionType.None;
            throw new Exception("error :: check_system");
        }

        /// <summary>
        /// Элементарные математические операции над матрицами
        /// </summary>
        private static void CheckSizes(DoubleMatrix left, DoubleMatrix right, string error_info = "")
        {
            if (left.NCols != right.NCols) throw new Exception($"{error_info} :: left operand cols ({left.NCols}) != left operand cols ({right.NCols})");
            if (left.NRows != right.NRows) throw new Exception($"{error_info} :: left operand rows ({left.NRows}) != left operand rows ({right.NRows})");
        }

        ///////////////////////////
        /////    Operator *   /////
        ///////////////////////////
        public static DoubleMatrix operator *(DoubleMatrix left, DoubleMatrix right)
        {
            if (left.NCols != right.NRows) throw new Exception("Error DoubleMatrix multiplication::a.NCols != b.NRows");

            DoubleMatrix b_t = Transpose(right);

            DoubleMatrix res = new DoubleMatrix(left.NRows, right.NCols);

            Parallel.For(0, left.NRows, i => { for (int j = 0; j < right.NCols; j++) res[i][j] = DoubleVector.Dot(left[i], b_t[j]); });

            return res;
        }

        public static DoubleVector operator * (DoubleMatrix left, DoubleVector right)
        {
            if (left.NCols != right.Count) throw new Exception("unable to DoubleMatrix and DoubleVector myltiply");
            return new DoubleVector(left.Combine(right, (l, r) => DoubleVector.Dot(l, r)));
        }

        public static DoubleVector operator * (DoubleVector left, DoubleMatrix right)
        {
            if (right.NRows != left.Count) throw new Exception("unable to DoubleMatrix and DoubleVector myltiply");
            DoubleVector result = new DoubleVector();
            for (int i = 0; i < right.NCols; i++) for (int j = 0; j < right.NRows; j++) result[i] += right[j][i] * left[i];
            return result;
        }

        public static DoubleMatrix operator * (DoubleMatrix left, double right)
        {
            DoubleMatrix result = new DoubleMatrix(left);
            result.Apply(v => v * right);
            return result;
        }

        public static DoubleMatrix operator * (double left, DoubleMatrix right) => right * left;
        
        ///////////////////////////
        /////    Operator +   /////
        ///////////////////////////
        public static DoubleMatrix operator + (DoubleMatrix left, DoubleMatrix right)
        {
            CheckSizes(left, right, "\"operator +\"");
            return new DoubleMatrix(left.Combine(right, (l, r) => l + r));
        }
       
        public static DoubleMatrix operator + (DoubleMatrix left, double right)
        {
            DoubleMatrix result = new DoubleMatrix(left);
            result.Apply(v => v + right);
            return result;
        }
        
        public static DoubleMatrix operator + (double left, DoubleMatrix right)
        {
            DoubleMatrix result = new DoubleMatrix(right);
            result.Apply(v => left + v);
            return result;
        }
    
        ///////////////////////////
        /////    Operator -   /////
        ///////////////////////////
        public static DoubleMatrix operator - (DoubleMatrix left, DoubleMatrix right)
        {
            CheckSizes(left, right, "\"operator -\"");
            return new DoubleMatrix(left.Combine(right, (l, r) => l - r));
        }
        
        public static DoubleMatrix operator - (DoubleMatrix left, double right)
        {
            DoubleMatrix result = new DoubleMatrix(left);
            result.Apply(v => v - right);
            return result;
        }
        
        public static DoubleMatrix operator - (double left, DoubleMatrix right)
        {
            DoubleMatrix result = new DoubleMatrix(right);
            result.Apply(v => left - v);
            return result;
        }

        ///////////////////////////
        /////    Operator /   /////
        ///////////////////////////
        public static DoubleMatrix operator / (DoubleMatrix left, DoubleMatrix right) => left * Invert(right);

        public static DoubleMatrix operator / (DoubleMatrix left, double right)
        {
            DoubleMatrix result = new DoubleMatrix(left);
            result.Apply(v => v / right);
            return result;
        }

        public static DoubleMatrix operator / (double left, DoubleMatrix right)
        {
            DoubleMatrix result = new DoubleMatrix(right);
            result.Apply(v => left / v);
            return result;
        }
    }
}

using System.Collections.Generic;
using System.Threading.Tasks;
using System;

namespace OptimizationMethods
{
    public enum SolutionType
    {
        Single = 0,
        Infinite = 1,
        None = 2
    }
    public class Matrix: TemplateVector<Vector>
    {
        /// <summary>
        /// Массив строк матрицы
        /// </summary>
        public Matrix AddCol(Vector col)
        {
            if (col.Count != NRows) throw new Exception("Error::AddCol::col.Size != NRows");
            for (int i = 0; i < Count; i++) this[i].PushBack(col[i]);
            return this;
        }

        public Matrix AddRow(Vector row)
        {
            if (row.Count != NCols) throw new Exception("Error::AddRow::row.Size != NCols");
            PushBack(row);
            return this;
        }

        public new Matrix PushBack(Vector v) 
        {
            if (NRows == 0) 
            {
                base.PushBack(v);
                return this;
            }
            if (v.Count != NCols) throw new Exception("Error::AddRow::row.Size != NCols");
            base.PushBack(v);
            return this;
        }

        public override string ToString() => $"{{\n  {string.Join(",\n  ", Map((v) => v.ToString()))}\n}}";

        /// <summary>
        ///  Количество строк
        /// </summary>
        public int NRows => Count;

        /// <summary>
        /// Количество столбцов
        /// </summary>
        public int NCols => NRows == 0 ? 0 : this[0].Count;

        public IEnumerable<Vector> Rows => this;

        /// <summary>
        /// Рамерность матрицы
        /// </summary>
        public int[] Size => new int[] { NRows, NCols };

        /// <summary>
        /// Конструктор матрицы из массива строк
        /// </summary>
        /// <param name="rows"></param>
        public Matrix() : base() { }
        
        public Matrix(params Vector[] rows): base()
        {
            if (rows == null) throw new Exception("Data is null...");

            if (rows.Length == 0) throw new Exception("Data is empty...");

            int maxSize = int.MinValue;
            
            int minSize = int.MaxValue;

            foreach (Vector v in rows)
            {
                if (v.Count > maxSize) maxSize = v.Count;
                if (v.Count < minSize) minSize = v.Count;
            }
            
            if (maxSize != minSize) throw new Exception("Incorrect matrix data");

            foreach (Vector r in rows) PushBack(r);
        }

        /// <summary>
        /// Конструктор матрцы по ее размерам и элементу по умолчанию
        /// </summary>
        /// <param name="n_rows">колическтво строк</param>
        /// <param name="n_cols">количество столбцов</param>
        /// <param name="defualtVal">значение элементов матрицы по умолчанию</param>
        public Matrix(int n_rows, int n_cols) : base()
        {
            for (int i = 0; i < n_rows; i++) PushBack(new Vector(n_cols));
        }
        
        /// <summary>
        /// Конструктор копирования
        /// </summary>
        /// <param name="original"></param>
        public Matrix(Matrix original):base()
        {
            foreach (Vector row in original) PushBack(new Vector(row));
        }

        /// <summary>
        /// </summary>
        /// <param name="f"></param>
        /// <param name="x"></param>
        /// <param name="eps"></param>
        /// <returns></returns>
        public static Matrix Hessian(FunctionND f, Vector x, double eps = 1e-6)
        {
            Matrix res = new Matrix(x.Count, x.Count);
            int row, col;
            for (row = 0; row < res.NRows; row++)
            {
                for (col = 0; col <= row; col++)
                {
                    res[row][col] = Vector.Partial2(f, x, row, col, eps);
                    res[col][row] = res[row][col];
                }
            }
            return res;
        }

        /// <summary>
        /// Определят ранг матрицы
        /// </summary>
        /// <param name="A"></param>
        /// <returns></returns>
        public static int Rank(Matrix A)
        {
            int n = A.NRows;

            int m = A.NCols;

            int rank = 0;

            bool[] row_selected = new bool[n];

            for (int i = 0; i < row_selected.Length; i++) row_selected[i] = false;

            for (int i = 0; i < m; i++)
            {
                int j;

                for (j = 0; j < n; j++) if (!row_selected[j] && Math.Abs(A[j][i]) > 1e-12) break;

                if (j != n)
                {
                    ++rank;

                    row_selected[j] = true;

                    for (int p = i + 1; p < m; p++) A[j][p] /= A[j][i];

                    for (int k = 0; k < n; k++)
                    {
                        if (k != j && Math.Abs(A[k][i]) > 1e-12)
                        {
                            for (int p = i + 1; p < m; p++) A[k][p] -= A[j][p] * A[k][i];
                        }
                    }
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
        public static Matrix Zeros(int n_rows, int n_cols) => new Matrix(n_rows, n_cols);

        /// <summary>
        /// Создаёт квадратную матрицу нулей
        /// </summary>
        /// <param name="size">сторона матрицы</param>
        /// <returns>квадратная матрицы нулей</returns>
        public static Matrix Zeros(int size) => Zeros(size, size);

        /// <summary>
        /// Создаёт единичную матрицу
        /// </summary>
        /// <param name="n_rows">число строк</param>
        /// <param name="n_cols">число столбцов</param>
        /// <returns>единичная матрица</returns>
        public static Matrix Identity(int n_rows, int n_cols)
        {
            Matrix I = new Matrix(n_rows, n_cols);
            for (int i = 0; i < Math.Min(n_rows, n_cols); i++) I[i][i] = 1.0;
            return I;
        }
        
        /// <summary>
        /// Создаёт квадратную единичную матрицу
        /// </summary>
        /// <param name="size">сторона матрицы</param>
        /// <returns>квадратная единичная матрица</returns>
        public static Matrix Identity(int size) => Identity(size, size);

        /// <summary>
        /// LU hазложение матрицы на нижнюю и верхнюю треугольные матрицы
        /// </summary>
        /// <param name="src">Матрица разложение которой нужно провести</param>
        /// <param name="low">Нижняя треугольная матрица</param>
        /// <param name="up">Верхняя треугольная матрица</param>
        public static void LU(ref Matrix src, out Matrix low, out Matrix up)
        {
            if (src.NCols != src.NRows) throw new Exception("LU decomposition error::non square matrix");

            low = new Matrix(src.NCols, src.NRows);

            up = new Matrix(src.NCols, src.NRows);

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
        private static Vector Linsolve(ref Matrix low, ref Matrix up, ref Vector b)
        {
            double det = 1.0;

            Vector x, z;

            for (int i = 0; i < up.NRows; i++) det *= (up[i][i] * up[i][i]);

            if (Math.Abs(det) < 1e-12) return null;
            
            z = new Vector(up.NRows);

            double tmp;

            for (int i = 0; i < z.Count; i++)
            {
                tmp = 0.0;

                for (int j = 0; j < i; j++) tmp += z[j] * low[i][j];
                
                z[i] = (b[i] - tmp) / low[i][i];
            }

            x = new Vector(up.NRows);

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
        public static Vector Linsolve(Matrix mat, Vector b)
        {
            if (mat.NRows != mat.NCols) throw new Exception("non square matrix");

            Matrix low, up;

            LU(ref mat, out low, out up);

            return Linsolve(ref low, ref up, ref b);
        }
        
        /// <summary>
        /// Рассчитывает обратную матрицу
        /// </summary>
        /// <param name="mat">матрица для которой ищем обратную</param>
        /// <returns>обратная матрица</returns>
        public static Matrix Invert(Matrix mat)
        {
            if (mat.NRows != mat.NCols) throw new Exception("non square matrix");

            Matrix low, up, inv;

            LU(ref mat, out low, out up);

            Vector b, col;

            b = new Vector(mat.NRows);

            inv = Zeros(mat.NRows);

            for (int i = 0; i < mat.NCols; i++)
            {
                b[i] = 1.0;
                col = Linsolve(ref low, ref up, ref b);
                if (col == null)    throw new Exception("unable to find matrix inversion");
                if (col.Count == 0) throw new Exception("unable to find matrix inversion");
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
        public static Matrix Transpose(Matrix mat)
        {
            Matrix trans = new Matrix(mat.NCols, mat.NRows);
            Parallel.For(0, mat.NRows, (i) =>
            {
                for (int j = 0; j < mat.NCols; j++) trans[j][i] = mat[i][j];
            });
            return trans;
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
        public static Matrix operator *(Matrix a, Matrix b)
        {
            if (a.NCols != b.NRows) throw new Exception("Error matrix multiplication::a.NCols != b.NRows");

            Matrix b_t = Transpose(b);

            Matrix res = new Matrix(a.NRows, b.NCols);

            Parallel.For(0, a.NRows, (i) => 
            {
                for (int j = 0; j < b.NCols; j++) res[i][j] = Vector.Dot(a[i], b_t[j]);
            });

            return res;
        }
   
        public static Vector operator *(Matrix mat, Vector vec)
        {
            if (mat.NCols != vec.Count) throw new Exception("unable to matrix and vector myltiply");
            Vector result = new Vector();
            foreach (Vector row in mat) result.PushBack(Vector.Dot(row, vec));
            return result;
        }
        
        public static Vector operator *(Vector vec, Matrix mat)
        {
            if (mat.NRows != vec.Count) throw new Exception("unable to matrix and vector myltiply");
            Vector result = new Vector();

            for (int i = 0; i < mat.NCols; i++)
            {
                for (int j = 0; j < mat.NRows; j++) result[i] += mat[j][i] * vec[i];
            }

            return result;
        }
        
        public static Matrix operator *(Matrix mat, double a)
        {
            Matrix result = new Matrix(mat);
            foreach (Vector row in mat) result.PushBack(a * row);
            return result;
        }
        
        public static Matrix operator *(double a, Matrix mat)
        {
            return mat * a;
        }
        
        public static Matrix operator +(Matrix a, Matrix b)
        {
            if (a.NCols != b.NCols) throw new Exception("unable to add matrix a to matrix b");
            if (a.NRows != b.NRows) throw new Exception("unable to add matrix a to matrix b");
            Matrix result = new Matrix();
            foreach (var pair in Zip(a, b)) result.PushBack(pair.First + pair.Second);
            return result;
        }
        
        public static Matrix operator +(Matrix a, double b)
        {
            Matrix result = new Matrix();
            foreach (Vector row in a) result.PushBack(b + row);
            return result;
        }
        
        public static Matrix operator +(double b, Matrix a)
        {
            return a + b;
        }
        
        public static Matrix operator -(Matrix a, Matrix b)
        {
            if (a.NCols != b.NCols) throw new Exception("unable to add matrix a to matrix b");
            if (a.NRows != b.NRows) throw new Exception("unable to add matrix a to matrix b");
            Matrix result = new Matrix();
            foreach (var pair in Zip(a, b)) result.PushBack(pair.First - pair.Second);
            return result;
        }
        
        public static Matrix operator -(Matrix a, double b)
        {
            Matrix result = new Matrix();
            foreach (Vector row in a) result.PushBack(row - b);
            return result;
        }
        
        public static Matrix operator -(double b, Matrix a)
        {
            Matrix result = new Matrix();
            foreach (Vector row in a) result.PushBack(b - row);
            return result;
        }
    }
}

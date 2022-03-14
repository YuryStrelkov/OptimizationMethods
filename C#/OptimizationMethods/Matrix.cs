using System;
using System.Diagnostics.CodeAnalysis;

namespace OptimizationMethods
{
    public class Matrix : IEquatable<Matrix>
    {
        /// <summary>
        /// проверка работоспособности класса
        /// </summary>
        public static void MatrixTest()
        {
            ///У меня всё работает, у вас мой класс тоже заработает
            Matrix matrix = new Vector[] { new double[] { 8, 1, 6 }, new double[] { 3, 5, 7 }, new double[] { 4, 9, 2 } };
            Console.WriteLine(matrix);
            Console.WriteLine("\nmatrix + matrix:\n" + (matrix + matrix));
            Console.WriteLine("\nmatrix - matrix:\n" + (matrix - matrix));
            Console.WriteLine("\nmatrix * matrix:\n" + (matrix * matrix));
            Matrix l, u;
            LU(ref matrix, out l, out u);
            Console.WriteLine("\nL  matrix:\n" + l);
            Console.WriteLine("\nU  matrix:\n" + u);
            Console.WriteLine("\nL * U - matrix:\n" + (l * u - matrix));
            Vector b = new double[] { 1, 2, 3 };
            /// x = {0.05,0.3,0.05};
            Vector x = Linsolve(matrix, b);
            Console.WriteLine("\nx vector:\n" + x);
            Console.WriteLine("\nAx - b:\n" + (matrix * x - b));
            Console.WriteLine("\nA*inv(A):\n" + matrix * Invert(matrix));
            Matrix matrix_ = new Vector[] { new double[] { 8, 1, 6 }, new double[] { 3, 5, 7 }};
            Console.WriteLine("\nnon rect mat:\n" + matrix_);
            Console.WriteLine("\nnon rect mat mul by transposed it self :\n" + matrix_*Transpose(matrix_));
        }
        /// <summary>
        /// Массив строк матрицы
        /// </summary>
        private Vector[] rows;
        /// <summary>
        /// Позволяет при иницилизации экземпляра класса вместо:
        /// Vector [] rows = new Vector[] { new double[] { 8, 1}, new double[] { 3, 5}, new double[] { 4, 9} };
        /// Matrix  m = new Matrix(rows);
        /// делать так:
        /// Matrix  m  = rows;
        /// </summary>
        /// <param name="rows"></param>
        public static implicit operator Matrix(Vector[] rows)
        {   
            return new Matrix(rows);
        }
        /// <summary>
        /// </summary>
        /// <param name="f"></param>
        /// <param name="x"></param>
        /// <param name="eps"></param>
        /// <returns></returns>
        public static Matrix Hessian(func_n f, Vector x, double eps = 1e-6)
        {
            Matrix res = new Matrix(x.Size, x.Size);
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
        /// Создаёт матрицу из нулей
        /// </summary>
        /// <param name="n_rows">число строк</param>
        /// <param name="n_cols">число столбцов</param>
        /// <returns>матрица нулей</returns>
        public static Matrix Zeros(int n_rows, int n_cols)
        {
            return new Matrix(n_rows, n_cols);
        }
        /// <summary>
        /// Создаёт квадратную матрицу нулей
        /// </summary>
        /// <param name="size">сторона матрицы</param>
        /// <returns>квадратная матрицы нулей</returns>
        public static Matrix Zeros(int size)
        {
            return Zeros(size, size);
        }
        /// <summary>
        /// Создаёт единичную матрицу
        /// </summary>
        /// <param name="n_rows">число строк</param>
        /// <param name="n_cols">число столбцов</param>
        /// <returns>единичная матрица</returns>
        public static Matrix Identity(int n_rows, int n_cols)
        {
            Matrix I = new Matrix(n_rows, n_cols);

            for (int i = 0; i < Math.Min(n_rows, n_cols); i++)
            {
                Vector v = I[i];
                v[i] = 1.0f;
                I[i] = v;
            }
            return I;
        }
        /// <summary>
        /// Создаёт квадратную единичную матрицу
        /// </summary>
        /// <param name="size">сторона матрицы</param>
        /// <returns>квадратная единичная матрица</returns>
        public static Matrix Identity(int size)
        {
            return Identity(size, size);
        }
        /// <summary>
        /// LU hазложение матрицы на нижнюю и верхнюю треугольные матрицы
        /// </summary>
        /// <param name="src">Матрица разложение которой нужно провести</param>
        /// <param name="low">Нижняя треугольная матрица</param>
        /// <param name="up">Верхняя треугольная матрица</param>
        public static void LU(ref Matrix src, out Matrix low, out Matrix up)
        {
            if (src.NCols != src.NRows)
            {
                throw new Exception("LU decomposition error::non square matrix");
            }

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

                        for (k = 0; k < i; k++)
                        {
                            low[j][i] = low[j][i] - low[j][k] * up[k][i];
                        }
                    }
                }

                for (j = 0; j < src.NRows; j++)
                {
                    if (j < i)
                    {
                        continue;
                    }
                    if (j == i)
                    {
                        up[i][j] = 1.0f;
                        continue;
                    }
                    up[i][j] = src[i][j] / low[i][i];

                    for (k = 0; k < i; k++)
                    {
                        up[i][j] = up[i][j] - ((low[i][k] * up[k][j]) / low[i][i]);
                    }
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
            double det = 1.0f;

            Vector x, z;

            for (int i = 0; i < up.NRows; i++)
            {
                det *= (up[i][i] * up[i][i]);
            }

            if (Math.Abs(det) < 1e-12f)
            {
                return null;
            }

            z = new Vector(up.NRows);

            double tmp;

            for (int i = 0; i < z.Size; i++)
            {
                tmp = 0.0f;
                for (int j = 0; j < i; j++)
                {
                    tmp += z[j] * low[i][j];
                }
                z[i] = (b[i] - tmp) / low[i][i];
            }

            x = new Vector(up.NRows);

            for (int i = z.Size - 1; i >= 0; i--)
            {
                tmp = 0.0f;
                for (int j = i + 1; j < z.Size; j++)
                {
                    tmp += x[j] * up[i][j];
                }
                x[i] = (z[i] - tmp);
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
            if (mat.NRows != mat.NCols)
            {
                throw new Exception("non square matrix");
            }

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
            if (mat.NRows != mat.NCols)
            {
                throw new Exception("non square matrix");
            }

            Matrix low, up, inv;

            LU(ref mat, out low, out up);

            Vector b, col;

            b = new Vector(mat.NRows);

            for (int i = 0; i < b.Size; i++)
            {
                b[i] = 0.0f;
            }

            inv = Zeros(mat.NRows);

            for (int i = 0; i < mat.NCols; i++)
            {
                b[i] = 1.0f;
                col = Linsolve(ref low, ref up, ref b);
                if (col == null)
                {
                    throw new Exception("unable to find matrix inversion");
                }
                if (col.Size == 0)
                {
                    throw new Exception("unable to find matrix inversion");
                }
                b[i] = 0.0f;
                for (int j = 0; j < mat.NRows; j++)
                {
                    inv[j][i] = col[j];
                }
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
            for (int i = 0; i < mat.NRows; i++)
            {
                for (int j = 0; j < mat.NCols; j++)
                {
                    trans[j][i] = mat[i][j];
                }
            }
            return trans;
        }
        /// <summary>
        /// Элементарные математические операции над матрицами
        /// </summary>
        public static Matrix operator *(Matrix a, Matrix b)
        {
            if (a.NCols != b.NRows)
            {
                throw new Exception("Error matrix multiplication::a.NCols != b.NRows");
            }

            Matrix b_t = Transpose(b);

            Matrix res = new Matrix(a.NRows, b.NCols);

            for (int i = 0; i < a.NRows; i++)
            {
                for (int j = 0; j < b.NCols; j++)
                {
                    res[i][j] = Vector.Dot(a[i], b_t[j]);
                }
            }
            return res;
        }
        public static Vector operator *(Matrix mat, Vector vec)
        {
            if (mat.NCols != vec.Size)
            {
                throw new Exception("unable to matrix and vector myltiply");
            }
            Vector result = new Vector(mat.NRows);
            int cntr = 0;
            foreach (Vector row in mat.rows)
            {
                result[cntr++] = Vector.Dot(row, vec);
            }
            return result;
        }
        public static Vector operator *(Vector vec, Matrix mat)
        {
            if (mat.NRows != vec.Size)
            {
                throw new Exception("unable to matrix and vector myltiply");
            }
            Vector result = new Vector(mat.NCols);

            for (int i = 0; i < mat.NCols; i++)
            {
                for (int j = 0; j < mat.NRows; j++)
                {
                    result[i] += mat[j][i] * vec[i];
                }
            }

            return result;
        }
        public static Matrix operator *(Matrix mat, double a)
        {
            Matrix result = new Matrix(mat);
            int cntr = 0;
            foreach (Vector row in mat.rows)
            {
                result[cntr++] *= a;
            }
            return result;
        }
        public static Matrix operator *(double a, Matrix mat)
        {
            return mat * a;
        }
        public static Matrix operator +(Matrix a, Matrix b)
        {
            if (a.NCols != b.NCols)
            {
                throw new Exception("unable to add matrix a to matrix b");
            }
            if (a.NRows != b.NRows)
            {
                throw new Exception("unable to add matrix a to matrix b");
            }

            Matrix result = new Matrix(a.NRows, a.NCols);

            for (int i = 0; i < a.NRows; i++)
            {
                result[i] = a[i] + b[i];
            }
            return result;
        }
        public static Matrix operator +(Matrix a, double b)
        {
            Matrix result = new Matrix(a.NRows, a.NCols);

            for (int i = 0; i < a.NRows; i++)
            {
                result[i] = a[i] + b;
            }
            return result;
        }
        public static Matrix operator +(double b, Matrix a)
        {
            return a + b;
        }
        public static Matrix operator -(Matrix a, Matrix b)
        {
            if (a.NCols != b.NCols)
            {
                throw new Exception("unable to add matrix a to matrix b");
            }
            if (a.NRows != b.NRows)
            {
                throw new Exception("unable to add matrix a to matrix b");
            }

            Matrix result = new Matrix(a.NRows, a.NCols);

            for (int i = 0; i < a.NRows; i++)
            {
                result[i] = a[i] - b[i];
            }
            return result;
        }
        public static Matrix operator -(Matrix a, double b)
        {
            Matrix result = new Matrix(a.NRows, a.NCols);

            for (int i = 0; i < a.NRows; i++)
            {
                result[i] = a[i] - b;
            }
            return result;
        }
        public static Matrix operator -(double b, Matrix a)
        {
            Matrix result = new Matrix(a.NRows, a.NCols);

            for (int i = 0; i < a.NRows; i++)
            {
                result[i] = b - a[i];
            }
            return result;
        }


        /// <summary>
        /// Сравнение матриц
        /// </summary>
        /// <param name="other"></param>
        /// <returns></returns>
        public bool Equals([AllowNull] Matrix other)
        {
            if (other.rows.Length != rows.Length)
            {
                return false;
            }
            for (int i = 0; i < rows.Length; i++)
            {
                if (!other[i].Equals(this[i]))
                {
                    return false;
                }
            }
            return true;
        }
        /// <summary>
        /// Базовое сравнение матриц
        /// </summary>
        /// <param name="other"></param>
        /// <returns></returns>
        public override bool Equals(object other)
        {
            if (!(other is Matrix))
            {
                return false;
            }
            return Equals(other as Matrix);
        }
        /// <summary>
        /// Не поверите, но расчитывает хеш-код, лол...
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode()
        {
            return HashCode.Combine(rows);
        }
        /// <summary>
        /// Строковое предствление матрицы
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            string s = "{\n";
            for (int i = 0; i < rows.Length - 1; i++)
            {
                s += " " + rows[i].ToString();
                s += ",\n";
            }
            s += " " + rows[rows.Length - 1].ToString();
            s += "\n}";
            return s;
        }
        /// <summary>
        /// Получение строки матрицы
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public Vector this[int id]
        {
            get
            {
                return rows[id];
            }
            set
            {
                rows[id] = value;
            }
        }
        /// <summary>
        ///  Количество строк
        /// </summary>
        public int NRows
        {
            get
            {
                return rows.Length;
            }
        }
        /// <summary>
        /// Количество столбцов
        /// </summary>
        public int NCols
        {
            get
            {
                if (NRows == 0)
                {
                    return 0;
                }
                return rows[0].Size;
            }
        }
        /// <summary>
        /// Рамерность матрицы
        /// </summary>
        public int[] Size
        {
            get
            {
                return new int[] { NRows, NCols };
            }
        }
        /// <summary>
        /// Конструктор матрицы из массива строк
        /// </summary>
        /// <param name="rows"></param>
        public Matrix(Vector[] rows)
        {
            if (rows == null)
            {
                throw new Exception("Data is null...");
            }

            if (rows.Length == 0)
            {
                throw new Exception("Data is empty...");
            }

            int row_size = rows[0].Size;

            for (int i = 0; i < rows.Length; i++)
            {
                if (rows[i].Size != row_size)
                {
                    throw new Exception("Incorrect matrix data");
                }
            }
            this.rows = rows;
        }
        /// <summary>
        /// Конструктор матрцы по ее размерам и элементу по умолчанию
        /// </summary>
        /// <param name="n_rows">колическтво строк</param>
        /// <param name="n_cols">количество столбцов</param>
        /// <param name="defualtVal">значение элементов матрицы по умолчанию</param>
        public Matrix(int n_rows, int n_cols, double defualtVal = 0.0f)
        {
            rows = new Vector[n_rows];

            for (int i = 0; i < n_rows; i++)
            {
                rows[i] = new Vector(n_cols, defualtVal);
            }
        }
        /// <summary>
        /// Конструктор копирования
        /// </summary>
        /// <param name="original"></param>
        public Matrix(Matrix original)
        {
            rows = new Vector[original.rows.Length];

            for (int i = 0; i < rows.Length; i++)
            {
                rows[i] = new Vector(original.rows[i]);
            }
        }
    }
}

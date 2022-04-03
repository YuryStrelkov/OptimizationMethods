import java.util.ArrayList;
import java.util.Objects;

public class Matrix {
    ArrayList<Vector>rows;

    public int rows()
    {
            return rows.size();
    }
    /// <summary>
    /// Количество столбцов
    /// </summary>
    public int cols()
    {
            if (rows() == 0)
            {
                return 0;
            }
            return rows.get(0).size();
    }

    public Matrix addCol(Vector col)throws Exception
    {
        if (col.size() != rows())
        {
            throw new Exception("Error::AddCol::col.Size != NRows");
        }
        for (int i = 0; i < rows.size(); i++)
        {
            rows.get(i).pushBack(col.get(i));
        }
        return this;
    }

    public Matrix addRow(Vector row)throws Exception
    {
        if (row.size() != cols())
        {
            throw new Exception("Error::AddRow::row.Size != NCols");
        }
        rows.add(row);
        return this;
    }

    @Override
    public String toString() {
        String s = "{\n";
        for (int i = 0; i < rows.size() - 1; i++)
        {
            s += " " + rows.get(i).toString();
            s += ",\n";
        }
        s += " " + rows.get(rows.size() - 1).toString();
        s += "\n}";
        return s;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Matrix matrix = (Matrix) o;
        return rows.equals(matrix.rows);
    }

    @Override
    public int hashCode() {
        return Objects.hash(rows);
    }

    public ArrayList<Vector> getRows() {
        return rows;
    }

    public int[]size()
    {
       return new int[] { rows(), cols() };
    }

    public double get(int i, int j)
    {
        return rows.get(i).get(j);
    }
    public void set(int i, int j, double value)
    {
        rows.get(i).set(j,value);
    }
    public Matrix(Vector[] rows)throws Exception
    {
        if (rows == null)
        {
            throw new Exception("Data is null...");
        }

        if (rows.length == 0)
        {
            throw new Exception("Data is empty...");
        }

        int row_size = rows[0].size();

        for (int i = 0; i < rows.length; i++)
        {
            if (rows[i].size() != row_size)
            {
                throw new Exception("Incorrect matrix data");
            }
        }
        this.rows = new ArrayList<>(rows.length);

        for (int i = 0; i < rows.length; i++)
        {
            this.rows.add(rows[i]);
        }
    }

    /// <summary>
    /// Конструктор матрцы по ее размерам и элементу по умолчанию
    /// </summary>
    /// <param name="n_rows">колическтво строк</param>
    /// <param name="n_cols">количество столбцов</param>
    /// <param name="defualtVal">значение элементов матрицы по умолчанию</param>
    public Matrix(int n_rows, int n_cols)
    {
        rows = new ArrayList(n_rows);

        for (int i = 0; i < n_rows; i++)
        {
            rows.add(new Vector(n_cols));
        }
    }
    /// <summary>
    /// Конструктор копирования
    /// </summary>
    /// <param name="original"></param>
    public Matrix(Matrix original)
    {
        rows = new ArrayList<>(original.rows.size());

        for (int i = 0; i < original.rows.size(); i++)
        {
            rows.add(new Vector(original.rows.get(i)));
        }
    }

    public static Matrix hessian(IFunctionND f, Vector x, double eps)throws Exception
    {
        Matrix res = new Matrix(x.size(), x.size());
        int row, col;
        double val;
        for (row = 0; row < res.rows(); row++)
        {
            for (col = 0; col <= row; col++)
            {
                val = Vector.partial2(f, x, row, col, eps);
                res.set(row,col,val);
                res.set(col,row,val);
            }
        }
        return res;
    }

    public static Matrix hessian(IFunctionND f, Vector x)throws Exception
    {
        return hessian( f,  x, 1e-5);
    }

    public static int rank(Matrix A)
    {
        int n = A.rows();

        int m = A.cols();

        int rank = 0;

        boolean[] row_selected = new boolean[n];

        for (int i = 0; i < row_selected.length; i++)
        {
            row_selected[i] = false;
        }

        for (int i = 0; i < m; i++)
        {
            int j;
            for (j = 0; j < n; j++)
            {
                if (!row_selected[j] && Math.abs(A.get(i,j)) > 1e-12)
                {
                    break;
                }
            }

            if (j != n)
            {
                ++rank;

                row_selected[j] = true;

                for (int p = i + 1; p < m; p++)
                {
                    A.set(j, p, A.get(j,p) / A.get(j, i));
                }

                for (int k = 0; k < n; k++)
                {
                    if (k != j && Math.abs(A.get(k,i)) > 1e-12)
                    {
                        for (int p = i + 1; p < m; p++)
                        {
                            A.set(k,p,A.get(k,p) - A.get(j,p) * A.get(k,i));
                        }
                    }
                }
            }
        }
        return rank;
    }

    public static Matrix zeros(int n_rows, int n_cols)
    {
        return new Matrix(n_rows, n_cols);
    }
    /// <summary>
    /// Создаёт квадратную матрицу нулей
    /// </summary>
    /// <param name="size">сторона матрицы</param>
    /// <returns>квадратная матрицы нулей</returns>
    public static Matrix zeros(int size)
    {
        return zeros(size, size);
    }
    /// <summary>
    /// Создаёт единичную матрицу
    /// </summary>
    /// <param name="n_rows">число строк</param>
    /// <param name="n_cols">число столбцов</param>
    /// <returns>единичная матрица</returns>
    public static Matrix identity(int n_rows, int n_cols)
    {
        Matrix I = new Matrix(n_rows, n_cols);

        for (int i = 0; i < Math.min(n_rows, n_cols); i++)
        {
            I.set(i,i,1.0);
        }
        return I;
    }

    /// <summary>
    /// LU hазложение матрицы на нижнюю и верхнюю треугольные матрицы
    /// </summary>
    /// <param name="src">Матрица разложение которой нужно провести</param>
    /// <param name="low">Нижняя треугольная матрица</param>
    /// <param name="up">Верхняя треугольная матрица</param>
    public static Matrix[] lu( Matrix src)throws Exception
    {
        Matrix low,  up;

        if (src.cols() != src.rows())
        {
            throw new Exception("LU decomposition error::non square matrix");
        }

        low = new Matrix(src.cols(), src.cols());

        up = new Matrix(src.cols(), src.cols());

        int i, j, k;

        for (i = 0; i < src.cols(); i++)
        {
            for (j = 0; j < src.cols(); j++)
            {
                if (j >= i)
                {
                    low.set(j,i,src.get(j,i));

                    for (k = 0; k < i; k++)
                    {
                        low.set(j,i,low.get(j,i) - low.get(j,k) * up.get(k,i));
                    }
                }
            }

            for (j = 0; j < src.cols(); j++)
            {
                if (j < i)
                {
                    continue;
                }
                if (j == i)
                {
                    up.set(i,j,1.0);
                    continue;
                }

                up.set(i,j,src.get(i,j) / low.get(i,i));

                for (k = 0; k < i; k++)
                {
                    up.set(i,j,up.get(i,j) - low.get(i,k) * up.get(k,j) / low.get(i,i));
                }
            }
        }
        return  new Matrix[]{low,up};
    }

    /// <summary>
    /// Вспомогательный метод рещения системы уравнений вида Ax = b при условии, что найдено разложение A = LU
    /// </summary>
    /// <param name="low">L</param>
    /// <param name="up">U</param>
    /// <param name="b">b</param>
    /// <returns>x</returns>
    private static Vector linsolve( Matrix low,  Matrix up,  Vector b)
    {
        double det = 1.0;

        Vector x, z;

        for (int i = 0; i < up.rows(); i++)
        {
            det *= (up.get(i,i) * up.get(i,i));
        }

        if (Math.abs(det) < 1e-12)
        {
            return null;
        }

        z = new Vector(up.rows());

        double tmp;

        for (int i = 0; i < z.size(); i++)
        {
            tmp = 0.0;
            for (int j = 0; j < i; j++)
            {
                tmp += z.get(j) * low.get(i,j);
            }
            z.set(i, (b.get(i) - tmp )/ low.get(i,i));
        }

        x = new Vector(up.rows());

        for (int i = z.size() - 1; i >= 0; i--)
        {
            tmp = 0.0;
            for (int j = i + 1; j < z.size(); j++)
            {
                tmp += x.get(j) * up.get(i,j);
            }
            x.set(i, z.get(i) - tmp);
        }
        return x;
    }

    /// <summary>
    /// Решение системы уравнений вида Ax = b
    /// </summary>
    /// <param name="mat">A</param>
    /// <param name="b">b</param>
    /// <returns>x</returns>
    public static Vector linsolve(Matrix mat, Vector b)throws Exception
    {
        if (mat.rows() != mat.cols())
        {
            throw new Exception("non square matrix");
        }

        Matrix low = null, up = null;

        Matrix[]lu_ = lu(mat);

        return linsolve( lu_[0],  lu_[1], b);
    }

    /// <summary>
    /// Рассчитывает обратную матрицу
    /// </summary>
    /// <param name="mat">матрица для которой ищем обратную</param>
    /// <returns>обратная матрица</returns>
    public static Matrix invert(Matrix mat)throws Exception
    {
        if (mat.rows() != mat.cols())
        {
            throw new Exception("non square matrix");
        }

        Matrix[]lu_ = lu( mat);

        Vector b, col;

        b = new Vector(mat.rows());

        Matrix inv = zeros(mat.rows());

        for (int i = 0; i < mat.cols(); i++)
        {
            b.set(i,1.0);
            col = linsolve( lu_[0], lu_[1], b);
            if (col == null)
            {
                throw new Exception("unable to find matrix inversion");
            }
            if (col.size() == 0)
            {
                throw new Exception("unable to find matrix inversion");
            }
            b.set(i,0.0);
            for (int j = 0; j < mat.rows(); j++)
            {
                inv.set(j,i,col.get(j));
            }
        }
        return inv;
    }

    /// <summary>
    /// Транспонирование матрицы
    /// </summary>
    /// <param name="mat"></param>
    /// <returns></returns>
    public static Matrix transpose(Matrix mat)
    {
        Matrix trans = new Matrix(mat.cols(), mat.rows());

        for (int i = 0; i < mat.rows(); i++)
        {
            for (int j = 0; j < mat.cols(); j++)
            {
                trans.set(j, i, mat.get(i,j));
            }
        }
        return trans;
    }
    public Matrix add(Matrix other)throws Exception
    {
        if(rows()!= other.rows())
        {
            throw new Exception("Dot product :: this.Size()!= other.Size()");
        }
        if(cols()!= other.rows())
        {
            throw new Exception("Dot product :: this.Size()!= other.Size()");
        }
        for (int i = 0;i<rows();i++)
        {
             rows.get(i).add(other.rows.get(i));
        }
        return  this;
    }
    public Matrix add(double other)
    {
        for (int i=0;i<rows();i++)
        {
            rows.get(i).add(other);
        }
        return  this;
    }
    public Matrix sub(Matrix other)throws Exception
    {
        if(rows()!= other.rows())
        {
            throw new Exception("Dot product :: this.Size()!= other.Size()");
        }
        if(cols()!= other.rows())
        {
            throw new Exception("Dot product :: this.Size()!= other.Size()");
        }
        for (int i = 0;i < rows();i++)
        {
            rows.get(i).sub(other.rows.get(i));
        }
        return  this;
    }
    public Matrix sub(double other)
    {
        for (int i=0;i<rows();i++)
        {
            rows.get(i).sub(other);
        }
        return  this;
    }
    public Matrix mul(double other)
    {
        for (int i = 0;i < rows();i++)
        {
            rows.get(i).mul(other);
        }
        return  this;
    }
    public Matrix div(double other)throws Exception
    {
        return  this.mul(1.0/other);
    }

    public static Matrix mul(Matrix a, Matrix b)throws Exception
    {
        if (a.cols() != b.rows())
        {
            throw new Exception("Error matrix multiplication::a.NCols != b.NRows");
        }

        Matrix b_t = transpose(b);

        Matrix res = new Matrix(a.rows(), b.cols());

        for (int i = 0; i < a.rows(); i++)
        {
            for (int j = 0; j < b.cols(); j++)
            {
                res.set(i,j,Vector.dot(a.rows.get(i), b_t.rows.get(j)));
            }
        }
        return res;
    }
    public static Vector mul(Matrix mat, Vector vec)throws Exception
    {
        if (mat.cols() != vec.size())
        {
            throw new Exception("unable to matrix and vector myltiply");
        }
        Vector result = new Vector(mat.rows());
        int cntr = 0;
        for (Vector row : mat.rows)
        {
            result.set(cntr++, Vector.dot(row, vec));
        }
        return result;
    }
    public static Vector mul(Vector vec, Matrix mat)throws Exception
    {
        if (mat.rows() != vec.size())
        {
            throw new Exception("unable to matrix and vector myltiply");
        }
        Vector result = new Vector(mat.cols());

        for (int i = 0; i < mat.cols(); i++)
        {
            for (int j = 0; j < mat.rows(); j++)
            {
                result.set(i,mat.get(j,i)* vec.get(i));
            }
        }

        return result;
    }
    public static Matrix mul(Matrix mat, double a)
    {
        Matrix result = new Matrix(mat);
        return result.mul(a);
    }
    public static Matrix operator(double a, Matrix mat)
    {
        return mul(mat , a);
    }
    public static Matrix add(Matrix a, Matrix b)throws Exception
    {
        if (a.cols() != b.cols())
        {
            throw new Exception("unable to add matrix a to matrix b");
        }
        if (a.rows() != b.rows())
        {
            throw new Exception("unable to add matrix a to matrix b");
        }

        Matrix result = new Matrix(a);

        return result.add(b);
    }
    public static Matrix add(Matrix a, double b)
    {
        Matrix result = new Matrix(a);

        return result.add(b);
    }
    public static Matrix add(double b, Matrix a)
    {
        return add(a , b);
    }
    public static Matrix sub(Matrix a, Matrix b)throws Exception
    {
        if (a.cols() != b.cols())
        {
            throw new Exception("unable to add matrix a to matrix b");
        }
        if (a.rows() != b.rows())
        {
            throw new Exception("unable to add matrix a to matrix b");
        }

        Matrix result = new Matrix(a);

        return result.sub(b);
    }
    public static Matrix sub(Matrix a, double b)
    {
        Matrix result = new Matrix(a);

        return result.sub(b);
    }
    public static Matrix sub(double b, Matrix a)
    {
        Matrix result = new Matrix(a);

        for (int i = 0; i < a.rows(); i++)
        {
            result.rows.set(i,Vector.sub(b,a.rows.get(i)));
        }
        return result;
    }
}

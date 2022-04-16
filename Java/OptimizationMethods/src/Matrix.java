import java.util.ArrayList;
import java.util.Objects;

enum SolutionType
{
    Single,
    Infinite,
    None
}

public class Matrix
{
    public static  boolean showMatrixDebugLog = false;

    private ArrayList<Vector>rows;

    public Vector row(int rowId)
    {
        return rows.get(rowId);
    }

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
            return row(0).size();
    }

    public Matrix addCol(Vector col)
    {
        if (col.size() != rows())
        {
            throw new RuntimeException("Error::AddCol::col.Size != NRows");
        }
        for (int i = 0; i < rows.size(); i++)
        {
            row(i).pushBack(col.get(i));
        }
        return this;
    }

    public Matrix addRow(Vector row)
    {
        if (row.size() != cols())
        {
            throw new RuntimeException("Error::AddRow::row.Size != NCols");
        }
        rows.add(row);
        return this;
    }

    @Override
    public String toString() {
        StringBuilder sb =new StringBuilder();
        sb.append("{\n");
        for (int i = 0; i < rows.size() - 1; i++)
        {
            sb.append(" " + rows.get(i).toString());
            sb.append(",\n");
        }
        sb.append(" " + rows.get(rows.size() - 1).toString());
        sb.append("\n}");
        return sb.toString();
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
        return row(i).get(j);
    }

    public void set(int i, int j, double value)
    {
        row(i).set(j,value);
    }

    public Matrix(Vector...  rows)
    {
        if (rows == null)
        {
            throw new RuntimeException("Data is null...");
        }

        if (rows.length == 0)
        {
            throw new RuntimeException("Data is empty...");
        }
        int rowSizeMax = Integer.MIN_VALUE;

        int rowSizeMin = Integer.MAX_VALUE;

        for(Vector row:rows)
        {
            if(row.size() > rowSizeMax)rowSizeMax = row.size();
            if(row.size() < rowSizeMin)rowSizeMin = row.size();
        }
        if(rowSizeMax!=rowSizeMin)throw new RuntimeException("Incorrect matrix data");

        this.rows = new ArrayList<>(rows.length);

        for(Vector row:rows)
        {
            this.rows.add(row);
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

    public static Matrix hessian(IFunctionND f, Vector x, double eps)
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

    public static Matrix hessian(IFunctionND f, Vector x)
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

    /// <summary>
    /// Проверяет совместность СЛАУ вида Ax = b. Используется теорема Кронекера-Капелли
    /// </summary>
    /// <param name="A"></param>
    /// <param name="b"></param>
    /// <returns>0 - нет решений, 1 - одно решение, 2 - бесконечное множествое решений</returns>
    public static SolutionType checkSystem(Matrix A, Vector b)
    {
        Matrix a = new Matrix(A);

        int rank_a = Matrix.rank(a);

        Matrix ab = new Matrix(A);

        int rank_a_b = Matrix.rank(ab.addCol(b));

        if(showMatrixDebugLog)
        {
            System.out.println("rank ( A ) " + rank_a + "\n");

            System.out.println("rank (A|b) " + rank_a_b + "\n");
            if (rank_a == rank_a_b)
            {
                System.out.println("one solution\n");
            }
            if (rank_a < rank_a_b)
            {
                System.out.println("infinite amount of solutions\n");
            }
            if (rank_a > rank_a_b)
            {
                System.out.println("no solutions\n");
            }
        }

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
        throw new RuntimeException("error :: check_system");
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
    public static Matrix[] lu( Matrix src)
    {
        Matrix low,  up;

        if (src.cols() != src.rows())
        {
            throw new RuntimeException("LU decomposition error::non square matrix");
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
            if(showMatrixDebugLog)
            {
                System.out.println("Matrix is probably singular :: unable to solve A^-1 b = x");
            }
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
    public static Vector linsolve(Matrix mat, Vector b)
    {
        if (mat.rows() != mat.cols())
        {
            throw new RuntimeException("non square matrix");
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
    public static Matrix invert(Matrix mat)
    {
        if (mat.rows() != mat.cols())
        {
            throw new RuntimeException("non square matrix");
        }

        Matrix[]lu_ = lu( mat);

        double det = 1.0;

        for (int i = 0; i < lu_[0].rows(); i++)
        {
            det *= (lu_[0].get(i,i) * lu_[0].get(i,i));
        }

        if (Math.abs(det) < 1e-12)
        {
            if(showMatrixDebugLog)
            {
                System.out.println("Matrix is probably singular :: unable to calculate invert matrix");
            }
            return null;
        }

        Vector b, col;

        b = new Vector(mat.rows());

        Matrix inv = zeros(mat.rows());

        for (int i = 0; i < mat.cols(); i++)
        {
            b.set(i,1.0);
            col = linsolve( lu_[0], lu_[1], b);
            if (col == null)
            {
                throw new RuntimeException("unable to find matrix inversion");
            }
            if (col.size() == 0)
            {
                throw new RuntimeException("unable to find matrix inversion");
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

    public Matrix add(Matrix other)
    {
        if(rows()!= other.rows())
        {
            throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
        }
        if(cols()!= other.rows())
        {
            throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
        }
        for (int i = 0; i < rows(); i++)
        {
             rows.get(i).add(other.rows.get(i));
        }
        return  this;
    }

    public Matrix add(double other)
    {
        for (int i = 0; i < rows(); i++)
        {
            rows.get(i).add(other);
        }
        return  this;
    }

    public Matrix sub(Matrix other)
    {
        if(rows()!= other.rows())
        {
            throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
        }
        if(cols()!= other.rows())
        {
            throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
        }
        for (int i = 0; i < rows(); i++)
        {
            rows.get(i).sub(other.rows.get(i));
        }
        return  this;
    }

    public Matrix sub(double other)
    {
        for (int i = 0; i < rows(); i++)
        {
            rows.get(i).sub(other);
        }
        return  this;
    }

    public Matrix mul(double other)
    {
        for (int i = 0; i < rows(); i++)
        {
            rows.get(i).mul(other);
        }
        return  this;
    }

    public Matrix div(double other)
    {
        return  this.mul(1.0 / other);
    }

    public static Matrix mul(Matrix a, Matrix b)
    {
        if (a.cols() != b.rows())
        {
            throw new RuntimeException("Error matrix multiplication::a.NCols != b.NRows");
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

    public static Vector mul(Matrix mat, Vector vec)
    {
        if (mat.cols() != vec.size())
        {
            throw new RuntimeException("unable to matrix and vector myltiply");
        }
        Vector result = new Vector(mat.rows());
        int cntr = 0;
        for (Vector row : mat.rows)
        {
            result.set(cntr++, Vector.dot(row, vec));
        }
        return result;
    }

    public static Vector mul(Vector vec, Matrix mat)
    {
        if (mat.rows() != vec.size())
        {
            throw new RuntimeException("unable to matrix and vector myltiply");
        }
        Vector result = new Vector(mat.cols());

        for (int i = 0; i < mat.cols(); i++)
        {
            for (int j = 0; j < mat.rows(); j++)
            {
                result.set(i, mat.get(j,i) * vec.get(i));
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

    public static Matrix add(Matrix a, Matrix b)
    {
        if (a.cols() != b.cols())
        {
            throw new RuntimeException("unable to add matrix a to matrix b");
        }
        if (a.rows() != b.rows())
        {
            throw new RuntimeException("unable to add matrix a to matrix b");
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

    public static Matrix sub(Matrix a, Matrix b)
    {
        if (a.cols() != b.cols())
        {
            throw new RuntimeException("unable to add matrix a to matrix b");
        }
        if (a.rows() != b.rows())
        {
            throw new RuntimeException("unable to add matrix a to matrix b");
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
            result.rows.set(i, Vector.sub(b, a.rows.get(i)));
        }
        return result;
    }
}

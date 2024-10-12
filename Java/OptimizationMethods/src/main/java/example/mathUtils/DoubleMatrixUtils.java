package example.mathUtils;

import example.functionalInterfaces.IFunctionND;

import java.util.Objects;

@SuppressWarnings("all")
public class DoubleMatrixUtils {
    /**
     * Транспонирование матрицы
     *
     * @param mat исходная матрица.
     * @return A^T.
     */
    public static DoubleMatrix transpose(DoubleMatrix mat) {
        DoubleMatrix trans = new DoubleMatrix(mat.colsCount(), mat.rowsCount());
        for (int row = 0; row < mat.rowsCount(); row++)
            for (int col = 0; col < mat.colsCount(); col++) trans.uncheckedSet(col, row, mat.uncheckedGet(row, col));
        return trans;
    }

    public static DoubleMatrix mexp(final DoubleMatrix matrix, final int steps) {
        DoubleMatrix result = identity(matrix.rowsCount(), matrix.colsCount());
        DoubleMatrix x_matrix = new DoubleMatrix(matrix);
        for (int index = 0; index < Math.min(NumericUtils.factorialsTable128.length, steps); index++) {
            result.add(mul(x_matrix, 1.0 / NumericUtils.factorialsTable128[index]));
            x_matrix = mul(x_matrix, x_matrix);
        }
        return result;
    }

    /**
     * Проверяет совместность СЛАУ вида Ax = b. Используется теорема Кронекера-Капелли
     *
     * @param A matrix
     * @param b vector
     * @return 0 - нет решений, 1 - одно решение, 2 - бесконечное множествое решений
     */
    public static SolutionType checkSystem(DoubleMatrix A, DoubleVector b) {
        DoubleMatrix a = new DoubleMatrix(A);

        int rank_a = rank(a);

        DoubleMatrix ab = new DoubleMatrix(A);

        int rank_a_b = rank(ab.addCol(b));

        if (NumericCommon.SHOW_MATRIX_DEBUG_LOG) {
            System.out.printf("rank ( A ) %s\n", rank_a);
            System.out.printf("rank ( A|b ) %s\n", rank_a_b);
            if (rank_a == rank_a_b) System.out.print("one solution\n");
            if (rank_a < rank_a_b) System.out.print("infinite amount of solutions\n");
            if (rank_a > rank_a_b) System.out.print("no solutions\n");
        }

        if (rank_a == rank_a_b) return SolutionType.Single;

        if (rank_a < rank_a_b) return SolutionType.Infinite;

        return SolutionType.None;
    }

    public static DoubleMatrix zeros(int n_rows, int n_cols) {
        return new DoubleMatrix(n_rows, n_cols);
    }

    /**
     * Создаёт квадратную матрицу нулей.
     *
     * @param size сторона матрицы.
     * @return матрица из нулей.
     */
    public static DoubleMatrix zeros(int size) {
        return zeros(size, size);
    }

    /**
     * Создаёт единичную матрицу.
     *
     * @param n_rows число строк.
     * @param n_cols число столбцов.
     * @return единичная матрица.
     */
    public static DoubleMatrix identity(int n_rows, int n_cols) {
        DoubleMatrix I = new DoubleMatrix(n_rows, n_cols);
        for (int i = 0; i < Math.min(n_rows, n_cols); i++) I.uncheckedSet(i, i, 1.0);
        return I;
    }

    /**
     * LU разложение матрицы на нижнюю и верхнюю треугольные матрицы
     * low - Нижняя треугольная матрица
     * up - Верхняя треугольная матрица
     *
     * @param src Матрица разложение которой нужно провести
     * @return массив из двух матриц, как рещультат LU разложения.
     */
    public static DoubleMatrix[] lu(DoubleMatrix src) {
        DoubleMatrix low, up;

        if (src.colsCount() != src.rowsCount()) throw new RuntimeException("LU decomposition error::non square matrix");

        low = new DoubleMatrix(src.colsCount(), src.colsCount());

        up = new DoubleMatrix(src.colsCount(), src.colsCount());

        int row, col, index;

        for (row = 0; row < src.colsCount(); row++) {
            for (col = 0; col < src.colsCount(); col++) {
                if (col >= row) {
                    low.uncheckedSet(col, row, src.uncheckedGet(col, row));
                    for (index = 0; index < row; index++)
                        low.uncheckedSet(col, row,low.uncheckedGet(col, row) -
                                low.uncheckedGet(col, index) * up.uncheckedGet(index, row));
                }
            }

            for (col = 0; col < src.colsCount(); col++) {
                if (col < row) continue;
                if (col == row) {
                    up.uncheckedSet(row, col, 1.0);
                    continue;
                }
                up.uncheckedSet(row, col, src.uncheckedGet(row, col) / low.uncheckedGet(row, row));
                for (index = 0; index < row; index++)
                    up.uncheckedSet(row, col,
                            up.uncheckedGet(row, col) - low.uncheckedGet(row, index) *
                                    up.uncheckedGet(index, col) / low.uncheckedGet(row, row));
            }
        }
        return new DoubleMatrix[]{low, up};
    }

    /**
     * Вспомогательный метод рещения системы уравнений вида Ax = b при условии, что найдено разложение A = LU.
     *
     * @param low нижняя треугольная матрица.
     * @param up  верхняя треугольняая матрица.
     * @param b   вектор свободных членов.
     * @return x = A^-1 * b = (L * U)^-1 * b.
     */
    private static DoubleVector linsolve(DoubleMatrix low, DoubleMatrix up, DoubleVector b) {
        double det = 1.0;
        DoubleVector x, z;
        for (int i = 0; i < up.rowsCount(); i++) det *= (up.uncheckedGet(i, i) * up.uncheckedGet(i, i));
        if (Math.abs(det) < NumericCommon.NUMERIC_ACCURACY_HIGH) {
            if (NumericCommon.SHOW_MATRIX_DEBUG_LOG)
                System.out.println("enjoy.mathUtils.Matrix is probably singular :: unable to solve A^-1 b = x");
            return null;
        }
        z = new DoubleVector(up.rowsCount());
        double tmp;
        int row, col;
        for (row = 0; row < z.size(); row++) {
            tmp = 0.0;
            for (col = 0; col < row; col++)
                tmp += z.uncheckedGet(col) * low.uncheckedGet(row, col);
            z.uncheckedSet(row, (b.get(row) - tmp) / low.uncheckedGet(row, row));
        }

        x = new DoubleVector(up.rowsCount());

        for (row = z.size() - 1; row >= 0; row--) {
            tmp = 0.0;
            for (col = row + 1; col < z.size(); col++)
                tmp += x.uncheckedGet(col) * up.uncheckedGet(row, col);
            x.uncheckedSet(row, z.get(row) - tmp);
        }
        return x;
    }

    /**
     * Решение системы уравнений вида Ax = b.
     *
     * @param mat матрица СЛАУ.
     * @param b   вектор свободных членов.
     * @return x = A^-1 * b.
     */
    public static DoubleVector linsolve(DoubleMatrix mat, DoubleVector b) {
        if (mat.rowsCount() != mat.colsCount()) throw new RuntimeException("non square matrix");
        DoubleMatrix[] lu_ = lu(mat);
        return linsolve(lu_[0], lu_[1], b);
    }

    /**
     * Рассчитывает обратную матрицу A^-1.
     *
     * @param mat исходная квадратная матрица.
     * @return A^-1.
     */
    public static DoubleMatrix invert(DoubleMatrix mat) {
        if (mat.rowsCount() != mat.colsCount()) throw new RuntimeException("non square matrix");

        DoubleMatrix[] lu_ = lu(mat);

        double det = 1.0;

        for (int rows = 0; rows < lu_[0].rowsCount(); rows++)
            det *= (lu_[0].uncheckedGet(rows, rows) * lu_[0].uncheckedGet(rows, rows));

        if (Math.abs(det) < NumericCommon.NUMERIC_ACCURACY_HIGH) {
            if (NumericCommon.SHOW_MATRIX_DEBUG_LOG)
                System.out.println("enjoy.mathUtils.Matrix is probably singular :: unable to calculate invert matrix");
            return null;
        }

        DoubleVector b, col;

        b = new DoubleVector(mat.rowsCount());

        DoubleMatrix inv = zeros(mat.rowsCount());

        for (int cols = 0; cols < mat.colsCount(); cols++) {
            b.set(cols, 1.0);
            col = linsolve(lu_[0], lu_[1], b);
            if (col == null) throw new RuntimeException("unable to find matrix inversion");
            if (col.size() == 0) throw new RuntimeException("unable to find matrix inversion");
            b.set(cols, 0.0);
            for (int rows = 0; rows < mat.rowsCount(); rows++)
                inv.uncheckedSet(rows, cols, col.get(rows));
        }
        return inv;
    }

    static void checkSizes(DoubleMatrix left, DoubleMatrix right) {
        if (left.rowsCount() != right.rowsCount()) throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
        if (left.colsCount() != right.rowsCount()) throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
    }

    public static DoubleMatrix hessian(IFunctionND f, DoubleVector x, double eps) {
        DoubleMatrix res = new DoubleMatrix(x.size(), x.size());
        // не учитывает симметричность матрицы
        // res.applyEnumerate((row, vec) -> {
        //     vec.applyEnumerate((col, v) -> DoubleVector.partial2(f, x, row, col, eps));
        //     return vec;
        // });
        int row, col;
        double val;
        for (row = 0; row < res.rowsCount(); row++) {
            for (col = 0; col <= row; col++) {
                val = DoubleVectorUtils.partial2(f, x, row, col, eps);
                res.uncheckedSet(row, col, val).uncheckedSet(col, row, val);
            }
        }
        return res;
    }

    public static DoubleMatrix hessian(IFunctionND f, DoubleVector x) {
        return hessian(f, x, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }

    public static int rank(DoubleMatrix A) {
        int n_rows = A.rowsCount();
        int m_cols = A.colsCount();
        int row, col;
        int rank = 0;
        boolean[] row_selected = new boolean[n_rows];

        for (row = 0; row < m_cols; row++) {
            for (col = 0; col < n_rows; col++)
                if (!row_selected[col] && Math.abs(A.uncheckedGet(row, col)) > NumericCommon.NUMERIC_ACCURACY_HIGH) break;
            if (col != n_rows) {
                ++rank;
                row_selected[col] = true;
                for (int p = row + 1; p < m_cols; p++)
                    A.uncheckedSet(col, p,
                            A.uncheckedGet(col, p) / A.uncheckedGet(col, row));

                for (int k = 0; k < n_rows; k++) {
                    if (k != col && Math.abs(A.uncheckedGet(k, row)) > NumericCommon.NUMERIC_ACCURACY_HIGH) {
                        for (int p = row + 1; p < m_cols; p++)
                            A.uncheckedSet(k, p,
                                    A.uncheckedGet(k, p) - A.uncheckedGet(col, p) * A.uncheckedGet(k, row));
                    }
                }
            }
        }
        return rank;
    }

    ///////////////////////////////////
    ////  MULTIPLICATION EXTERNAL  ////
    ///////////////////////////////////
    public static DoubleMatrix mul(DoubleMatrix left, DoubleMatrix right) {
        if (left.colsCount() != right.rowsCount()) throw new RuntimeException("Error matrix multiplication::a.NCols != b.NRows");
        // Возможно версия в лоб для наших целей будет быстрее, чем транспонирование и поэлементное
        // скалярное произведение... Но последнее много изящнее, да...
        DoubleMatrix result = new DoubleMatrix(left.rowsCount(), right.colsCount());
        int rowIndex, colIndex, index;
        for (rowIndex = 0; rowIndex < left.rowsCount(); rowIndex++) {
            final DoubleVector row = left.get(rowIndex);
            for (colIndex = 0; colIndex < right.colsCount(); colIndex++) {
                double result_ij = 0.0;
                for (index = 0; index < right.rowsCount(); index++)
                    result_ij += right.uncheckedGet(index, colIndex) * row.uncheckedGet(index);
                result.uncheckedSet(rowIndex, colIndex, result_ij);
            }
        }
        return result;
        // DoubleMatrix right_t = transpose(right);
        // return new DoubleMatrix(map(left, vec -> new DoubleVector(map(right_t, col -> col.dot(vec)))));
    }

    public static DoubleVector mul(DoubleMatrix left, DoubleVector right) {
        if (left.colsCount() != right.size()) throw new RuntimeException("unable to matrix and vector multiply");
        return new DoubleVector(TemplateVector.map(left, row -> row.dot(right)));
    }

    public static DoubleVector mul(DoubleVector left, DoubleMatrix right) {
        if (right.rowsCount() != left.size()) throw new RuntimeException("unable to matrix and vector multiply");
        DoubleVector result = new DoubleVector(right.colsCount());
        for (int row = 0; row < right.colsCount(); row++) {
            final double rowValue = left.uncheckedGet(row);
            for (int col = 0; col < right.rowsCount(); col++)
                result.uncheckedSet(row, right.get(col, row) * rowValue);
        }
        return result;
    }

    public static DoubleMatrix mul(DoubleMatrix left, double right) {
        return new DoubleMatrix(TemplateVector.map(left, row -> (DoubleVectorUtils.mul(row, right))));
    }

    public static DoubleMatrix mul(double left, DoubleMatrix right) {
        return new DoubleMatrix(TemplateVector.map(right, row -> (DoubleVectorUtils.mul(left, row))));
    }

    ///////////////////////////////////
    ////     ADDITION EXTERNAL     ////
    ///////////////////////////////////
    public static DoubleMatrix add(DoubleMatrix left, DoubleMatrix right) {
        checkSizes(left, right);
        return new DoubleMatrix(TemplateVector.combine(left, right, (l, r) -> DoubleVectorUtils.add(l, r)));
    }

    public static DoubleMatrix add(DoubleMatrix left, double right) {
        return new DoubleMatrix(TemplateVector.map(left, row -> (DoubleVectorUtils.add(row, right))));
    }

    public static DoubleMatrix add(double left, DoubleMatrix right) {
        return new DoubleMatrix(TemplateVector.map(right, row -> (DoubleVectorUtils.add(left, row))));
    }

    ///////////////////////////////////
    ////    SUBTRACTION EXTERNAL   ////
    ///////////////////////////////////
    public static DoubleMatrix sub(DoubleMatrix left, DoubleMatrix right) {
        checkSizes(left, right);
        return new DoubleMatrix(TemplateVector.combine(left, right, (l, r) -> DoubleVectorUtils.sub(l, r)));
    }

    public static DoubleMatrix sub(DoubleMatrix left, double right) {
        return new DoubleMatrix(TemplateVector.map(left, row -> (DoubleVectorUtils.sub(row, right))));
    }

    public static DoubleMatrix sub(double left, DoubleMatrix right) {
        return new DoubleMatrix(TemplateVector.map(right, row -> (DoubleVectorUtils.sub(left, row))));
    }

    ///////////////////////////////////
    ////     DIVISION EXTERNAL     ////
    ///////////////////////////////////
    public static DoubleMatrix div(DoubleMatrix left, DoubleMatrix right) {
        return mul(left, Objects.requireNonNull(invert(right)));
    }

    public static DoubleVector div(DoubleMatrix left, DoubleVector right) {
        if (left.colsCount() != right.size()) throw new RuntimeException("unable to matrix and vector multiply");
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l.dot(r)));
    }

    public static DoubleVector div(DoubleVector left, DoubleMatrix right) {
        return mul(Objects.requireNonNull(invert(right)), left);
    }

    public static DoubleMatrix div(DoubleMatrix left, double right) {
        return mul(left, 1.0 / right);
    }

    public static DoubleMatrix div(double left, DoubleMatrix right) {
        return new DoubleMatrix(TemplateVector.map(right, row -> (DoubleVectorUtils.div(left, row))));
    }
}

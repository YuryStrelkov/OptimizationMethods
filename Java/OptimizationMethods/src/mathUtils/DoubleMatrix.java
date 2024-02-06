package mathUtils;
import functionalInterfaces.IFunctionND;

import java.util.Arrays;
import java.util.Iterator;
import java.util.Objects;

enum SolutionType {
    Single,
    Infinite,
    None
}

public class DoubleMatrix extends TemplateVector<DoubleVector> {
    private static int[] matrixSizeInfo(Iterator<DoubleVector> rows)
    {
        int[] info = new int[]{-1, -1}; // rows, cols
        if (rows == null) return info;
        int rowSizeMax = Integer.MIN_VALUE;
        int rowSizeMin = Integer.MAX_VALUE;
        int n_rows = 0;
        while (rows.hasNext())
        {
            var row = rows.next();
            n_rows++;
            if (row.size() > rowSizeMax) rowSizeMax = row.size();
            if (row.size() < rowSizeMin) rowSizeMin = row.size();
        }
        if (n_rows == 0 || rowSizeMax != rowSizeMin) return info;
        info[0] = n_rows;
        info[1] = rowSizeMin;
        return info;
    }

    protected DoubleMatrix(Iterable<DoubleVector> rows) {
        super();
        for (DoubleVector v : rows) pushBack((DoubleVector) v.clone());
    }

    public DoubleMatrix(DoubleVector... rows) {
        super();
        int[] shape = matrixSizeInfo(Arrays.stream(rows).iterator());
        if(shape[0] == -1) throw new RuntimeException("Matrix(DoubleVector... rows)::Data is wring...");
        for (DoubleVector v : rows) pushBack((DoubleVector) v.clone());
    }

    /**
     * Конструктор матрцы по ее размерам и элементу по умолчанию.
     *
     * @param n_rows колическтво строк.
     * @param n_cols количество столбцов.
     */
    public DoubleMatrix(int n_rows, int n_cols) {
        super();
        for (int i = 0; i < n_rows; i++) pushBack(new DoubleVector(n_cols));
    }

    /**
     * Конструктор копирования.
     *
     * @param original исходная матрица.
     */
    public DoubleMatrix(DoubleMatrix original) {
        super();
        for (DoubleVector v : original) pushBack((DoubleVector) v.clone());
    }

    @Override
    public String toString() {
        return String.format("{\n\t%s\n}", String.join(";\n\t", DoubleMatrix.map(this, Object::toString)));
    }

    /**
     * Строка матрицы.
     *
     * @param rowId номер строки.
     * @return строка матрицы.
     */
    public DoubleVector row(int rowId) {
        return get(rowId);
    }

    /**
     * Вектор строк матрицы.
     *
     * @return вектор строк матрицы.
     */
    public TemplateVector<DoubleVector> getRows() {
        return this;
    }

    /**
     * Количество строк.
     *
     * @return Количество строк.
     */
    public int rows() {
        return super.size();
    }

    /**
     * Количество столбцов.
     *
     * @return Количество столбцов.
     */
    public int cols() {
        if (rows() == 0) return 0;
        return row(0).size();
    }

    /**
     * Добавляет новый столбец к матрице.
     *
     * @param col новый стобец.
     * @return обновлённая матрица.
     */
    public DoubleMatrix addCol(DoubleVector col) {
        if (col == null) return this;
        if (col.size() != rows()) throw new RuntimeException("Error::AddCol::col.Size != NRows");
        int index = 0;
        for (DoubleVector row : this) {
            row.pushBack(col.get(index));
            index++;
        }
        return this;
    }

    /**
     * Добавляет новую строку к матрице.
     *
     * @param row новая строка.
     * @return обновлённая матрица.
     */
    public DoubleMatrix addRow(DoubleVector row) {
        if (row == null) return this;
        if (row.size() != cols()) throw new RuntimeException("Error::AddRow::row.Size != NCols");
        pushBack(row);
        return this;
    }

    /**
     * Размерность матрицы.
     *
     * @return массив из целых чисел (количество строк, количество столбцов).
     */
    public final int[] shape() {
        return new int[]{rows(), cols()};
    }

    /**
     * Элемент матрицы
     *
     * @param row номер строки
     * @param col номер столбца
     * @return элемент матрицы[row, col]
     */
    public double get(int row, int col) {
        return get(row).get(col);
    }

    protected double unchecked_get(int row, int col) {
        return unchecked_get(row).unchecked_get(col);
    }

    /**
     * Устанавливает новое значение элемента матрицы
     * элемент матрицы[row, col] = value
     *
     * @param row   номер строки
     * @param col   номер столбца
     * @param value новое значение элемента матрицы
     * @return обновлённая матрица.
     */
    public DoubleMatrix set(int row, int col, double value) {
        get(row).set(col, value);
        return this;
    }

    protected DoubleMatrix unchecked_set(int row, int col, double value) {
        unchecked_get(row).unchecked_set(col, value);
        return this;
    }

    public static DoubleMatrix hessian(IFunctionND f, DoubleVector x, double eps) {
        DoubleMatrix res = new DoubleMatrix(x.size(), x.size());
        int row, col;
        double val;
        for (row = 0; row < res.rows(); row++) {
            for (col = 0; col <= row; col++) {
                val = DoubleVector.partial2(f, x, row, col, eps);
                res.unchecked_set(row, col, val);
                res.unchecked_set(col, row, val);
            }
        }
        return res;
    }

    public static DoubleMatrix hessian(IFunctionND f, DoubleVector x) {
        return hessian(f, x, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }

    public static int rank(DoubleMatrix A) {
        int n = A.rows();

        int m = A.cols();

        int rank = 0;

        boolean[] row_selected = new boolean[n];

        for (int i = 0; i < m; i++) {
            int j;

            for (j = 0; j < n; j++)
                if (!row_selected[j] && Math.abs(A.unchecked_get(i, j)) > NumericCommon.NUMERIC_ACCURACY_HIGH) break;

            if (j != n) {
                ++rank;

                row_selected[j] = true;

                for (int p = i + 1; p < m; p++) A.unchecked_set(j, p,
                        A.unchecked_get(j, p) / A.unchecked_get(j, i));

                for (int k = 0; k < n; k++) {
                    if (k != j && Math.abs(A.unchecked_get(k, i)) > NumericCommon.NUMERIC_ACCURACY_HIGH) {
                        for (int p = i + 1; p < m; p++) A.unchecked_set(k, p,
                                A.unchecked_get(k, p) - A.unchecked_get(j, p) * A.unchecked_get(k, i));
                    }
                }
            }
        }
        return rank;
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

        int rank_a = DoubleMatrix.rank(a);

        DoubleMatrix ab = new DoubleMatrix(A);

        int rank_a_b = DoubleMatrix.rank(ab.addCol(b));

        if (NumericCommon.SHOW_MATRIX_DEBUG_LOG) {
            System.out.println("rank ( A ) " + rank_a + "\n");

            System.out.println("rank (A|b) " + rank_a_b + "\n");
            if (rank_a == rank_a_b) System.out.println("one solution\n");

            if (rank_a < rank_a_b) System.out.println("infinite amount of solutions\n");

            if (rank_a > rank_a_b) System.out.println("no solutions\n");
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
        for (int i = 0; i < Math.min(n_rows, n_cols); i++) I.unchecked_set(i, i, 1.0);
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

        if (src.cols() != src.rows()) throw new RuntimeException("LU decomposition error::non square matrix");

        low = new DoubleMatrix(src.cols(), src.cols());

        up = new DoubleMatrix(src.cols(), src.cols());

        int i, j, k;

        for (i = 0; i < src.cols(); i++) {
            for (j = 0; j < src.cols(); j++) {
                if (j >= i) {
                    low.unchecked_set(j, i, src.unchecked_get(j, i));

                    for (k = 0; k < i; k++) low.unchecked_set(j, i,
                            low.unchecked_get(j, i) - low.unchecked_get(j, k) * up.unchecked_get(k, i));
                }
            }

            for (j = 0; j < src.cols(); j++) {
                if (j < i) continue;

                if (j == i) {
                    up.unchecked_set(i, j, 1.0);
                    continue;
                }

                up.unchecked_set(i, j, src.unchecked_get(i, j) / low.unchecked_get(i, i));

                for (k = 0; k < i; k++) up.unchecked_set(i, j,
                        up.unchecked_get(i, j) - low.unchecked_get(i, k) *
                                up.unchecked_get(k, j) / low.unchecked_get(i, i));
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

        for (int i = 0; i < up.rows(); i++) det *= (up.unchecked_get(i, i) * up.unchecked_get(i, i));

        if (Math.abs(det) < NumericCommon.NUMERIC_ACCURACY_HIGH) {
            if (NumericCommon.SHOW_MATRIX_DEBUG_LOG)
                System.out.println("mathUtils.Matrix is probably singular :: unable to solve A^-1 b = x");
            return null;
        }

        z = new DoubleVector(up.rows());

        double tmp;

        for (int i = 0; i < z.size(); i++) {
            tmp = 0.0;
            for (int j = 0; j < i; j++) tmp += z.unchecked_get(j) * low.unchecked_get(i, j);
            z.unchecked_set(i, (b.get(i) - tmp) / low.unchecked_get(i, i));
        }

        x = new DoubleVector(up.rows());

        for (int i = z.size() - 1; i >= 0; i--) {
            tmp = 0.0;
            for (int j = i + 1; j < z.size(); j++) tmp += x.unchecked_get(j) * up.unchecked_get(i, j);
            x.unchecked_set(i, z.get(i) - tmp);
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
        if (mat.rows() != mat.cols()) throw new RuntimeException("non square matrix");
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
        if (mat.rows() != mat.cols()) throw new RuntimeException("non square matrix");

        DoubleMatrix[] lu_ = lu(mat);

        double det = 1.0;

        for (int i = 0; i < lu_[0].rows(); i++) det *= (lu_[0].unchecked_get(i, i) * lu_[0].unchecked_get(i, i));

        if (Math.abs(det) < NumericCommon.NUMERIC_ACCURACY_HIGH) {
            if (NumericCommon.SHOW_MATRIX_DEBUG_LOG)
                System.out.println("mathUtils.Matrix is probably singular :: unable to calculate invert matrix");
            return null;
        }

        DoubleVector b, col;

        b = new DoubleVector(mat.rows());

        DoubleMatrix inv = zeros(mat.rows());

        for (int i = 0; i < mat.cols(); i++) {
            b.set(i, 1.0);
            col = linsolve(lu_[0], lu_[1], b);

            if (col == null) throw new RuntimeException("unable to find matrix inversion");

            if (col.size() == 0) throw new RuntimeException("unable to find matrix inversion");

            b.set(i, 0.0);

            for (int j = 0; j < mat.rows(); j++) inv.unchecked_set(j, i, col.get(j));
        }
        return inv;
    }

    /**
     * Транспонирование матрицы
     *
     * @param mat исходная матрица.
     * @return A^T.
     */
    public static DoubleMatrix transpose(DoubleMatrix mat) {
        DoubleMatrix trans = new DoubleMatrix(mat.cols(), mat.rows());
        for (int i = 0; i < mat.rows(); i++)
            for (int j = 0; j < mat.cols(); j++) trans.unchecked_set(j, i, mat.unchecked_get(i, j));
        return trans;
    }

    private static void checkSizes(DoubleMatrix left, DoubleMatrix right)
    {
        if (left.rows() != right.rows()) throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
        if (left.cols() != right.rows()) throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
    }

    ///////////////////////////////
    //      ADDITION INTERNAL    //
    ///////////////////////////////
    public DoubleMatrix add(DoubleMatrix other) {
        checkSizes(this, other);
        combine(other, (l, r) -> l.add(r));
        return this;
    }

    public DoubleMatrix add(double other) {
        apply(v -> v.add(other));
        return this;
    }

    ///////////////////////////////
    //    SUBTRACTION INTERNAL   //
    ///////////////////////////////
    public DoubleMatrix sub(DoubleMatrix other) {
        checkSizes(this, other);
        combine(other, (l, r) -> l.sub(r));
        return this;
    }

    public DoubleMatrix sub(double other) {
        apply(v -> v.sub(other));
        return this;
    }

    ///////////////////////////////
    //  MULTIPLICATION INTERNAL  //
    ///////////////////////////////
    public DoubleMatrix mul(double other) {
        apply(v -> v.mul(other));
        return this;
    }
    // public mathUtils.Matrix mul(mathUtils.Matrix other) ...

    ///////////////////////////////
    //     DIVISION INTERNAL     //
    ///////////////////////////////
    public DoubleMatrix div(double other) {
        return this.mul(1.0 / other);
    }
    // public mathUtils.Matrix div(mathUtils.Matrix other) ...

    ///////////////////////////////////
    ////  MULTIPLICATION EXTERNAL  ////
    ///////////////////////////////////
    public static DoubleMatrix mul(DoubleMatrix left, DoubleMatrix right) {
        if (left.cols() != right.rows()) throw new RuntimeException("Error matrix multiplication::a.NCols != b.NRows");
        DoubleMatrix b_t = transpose(right);
        DoubleMatrix res = new DoubleMatrix(left.rows(), right.cols());
        for (int i = 0; i < left.rows(); i++)
            for (int j = 0; j < right.cols(); j++)
                res.set(i, j, DoubleVector.dot(left.row(i), b_t.row(j)));
        return res;
    }

    public static DoubleVector mul(DoubleMatrix left, DoubleVector right) {
        if (left.cols() != right.size()) throw new RuntimeException("unable to matrix and vector multiply");
        return new DoubleVector(combine(left, right, (l, r) -> l.dot(r)));
    }

    public static DoubleVector mul(DoubleVector left, DoubleMatrix right) {
        if (right.rows() != left.size()) throw new RuntimeException("unable to matrix and vector multiply");

        DoubleVector result = new DoubleVector(right.cols());

        for (int i = 0; i < right.cols(); i++) {
            for (int j = 0; j < right.rows(); j++) result.set(i, right.get(j, i) * left.get(i));
        }

        return result;
    }

    public static DoubleMatrix mul(DoubleMatrix left, double right) {
        return new DoubleMatrix(map(left, row -> (DoubleVector.mul(row, right))));
    }

    public static DoubleMatrix mul(double left, DoubleMatrix right) {
        return new DoubleMatrix(map(right, row -> (DoubleVector.mul(left, row))));
    }

    ///////////////////////////////////
    ////     ADDITION EXTERNAL     ////
    ///////////////////////////////////
    public static DoubleMatrix add(DoubleMatrix left, DoubleMatrix right) {
        checkSizes(left, right);
        return new DoubleMatrix(combine(left, right, (l, r) -> DoubleVector.add(l, r)));
    }

    public static DoubleMatrix add(DoubleMatrix left, double right) {
        return new DoubleMatrix(map(left, row -> (DoubleVector.add(row, right))));
    }

    public static DoubleMatrix add(double left, DoubleMatrix right) {
        return new DoubleMatrix(map(right, row -> (DoubleVector.add(left, row))));
    }

    ///////////////////////////////////
    ////    SUBTRACTION EXTERNAL   ////
    ///////////////////////////////////
    public static DoubleMatrix sub(DoubleMatrix left, DoubleMatrix right) {
        checkSizes(left, right);
        return new DoubleMatrix(combine(left, right, (l, r) -> DoubleVector.sub(l, r)));
    }

    public static DoubleMatrix sub(DoubleMatrix left, double right) {
        return new DoubleMatrix(map(left, row -> (DoubleVector.sub(row, right))));
    }

    public static DoubleMatrix sub(double left, DoubleMatrix right) {
        return new DoubleMatrix(map(right, row -> (DoubleVector.sub(left, row))));
    }

    ///////////////////////////////////
    ////     DIVISION EXTERNAL     ////
    ///////////////////////////////////
    public static DoubleMatrix div(DoubleMatrix left, DoubleMatrix right) {
        return mul(left, Objects.requireNonNull(invert(right)));
    }

    public static DoubleVector div(DoubleMatrix left, DoubleVector right) {
        if (left.cols() != right.size()) throw new RuntimeException("unable to matrix and vector multiply");
        return new DoubleVector(combine(left, right, (l, r) -> l.dot(r)));
    }

    public static DoubleVector div(DoubleVector left, DoubleMatrix right) {
        return mul(Objects.requireNonNull(invert(right)), left);
    }

    public static DoubleMatrix div(DoubleMatrix left, double right) {
        return mul(left, 1.0 / right);
    }

    public static DoubleMatrix div(double left, DoubleMatrix right) {
        return new DoubleMatrix(map(right, row -> (DoubleVector.div(left, row))));
    }
}

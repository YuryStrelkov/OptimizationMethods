package mathUtils;

import mathUtils.functionalInterfaces.IFunctionND;
import java.util.Objects;


enum SolutionType {
    Single,
    Infinite,
    None
}


@SuppressWarnings("all")
public final class DoubleMatrix extends TemplateVector<DoubleVector> {

    protected DoubleMatrix(Slice rows, DoubleMatrix source) {
        super(rows, source);
    }

    protected DoubleMatrix(Slice rows, Slice cols, DoubleMatrix source) {
        map(new DoubleMatrix(rows, source), col -> col.get(cols));
    }

    protected DoubleMatrix(Iterable<DoubleVector> rows) {
        super();
        for (DoubleVector v : rows) pushBack(new DoubleVector(v));
    }

    public DoubleMatrix(DoubleVector... rows) {
        super();
        if (rows.length == 0) return;
        int rowSize = Integer.MAX_VALUE;
        for (final DoubleVector row : rows) rowSize = Math.min(rowSize, row.size());
        for (DoubleVector v : rows) pushBack(new DoubleVector(v.get(new Slice(0, rowSize))));
        //applyEnumerate(rows, (index, row)->{row.pushBack(col.get(index)); return row;});
    }

    /**
     * Конструктор матрцы по ее размерам и элементу по умолчанию.
     *
     * @param n_rows колическтво строк.
     * @param n_cols количество столбцов.
     */
    public DoubleMatrix(int n_rows, int n_cols) {
        super(n_rows);
        fill(i -> new DoubleVector(n_cols));
    }

    /**
     * Конструктор копирования.
     *
     * @param original исходная матрица.
     */
    public DoubleMatrix(DoubleMatrix original) {
        super(original.rowsCount());
        apply(original, DoubleVector::new);
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
    public int rowsCount() {
        return super.size();
    }

    /**
     * Количество столбцов.
     *
     * @return Количество столбцов.
     */
    public int colsCount() {
        if (rowsCount() == 0) return 0;
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
        if (col.size() != rowsCount()) throw new RuntimeException("Error::AddCol::col.Size != NRows");
        applyEnumerate((index, row) -> {
            row.pushBack(col.get(index));
            return row;
        });
        return this;
    }

    /**
     * Добавляет новый столбец к матрице.
     *
     * @param col новый стобец.
     * @return обновлённая матрица.
     */
    public DoubleMatrix addCol() {
        applyEnumerate((index, row) -> { row.pushBack(0.0); return row;});
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
        if (row.size() != colsCount()) throw new RuntimeException("Error::AddRow::row.Size != NCols");
        pushBack(row);
        return this;
    }
    public DoubleMatrix addRow() {
        pushBack(new DoubleVector(colsCount()));
        return this;
    }

    /**
     * Размерность матрицы.
     *
     * @return массив из целых чисел (количество строк, количество столбцов).
     */
    public final int[] shape() {
        return new int[]{rowsCount(), colsCount()};
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

    public DoubleMatrix get(Slice slice) {
        return new DoubleMatrix(slice, this);
    }

    public DoubleMatrix get(Slice rows, Slice cols) {
        return new DoubleMatrix(rows, cols, this);
    }

    protected double uncheckedGet(int row, int col) {
        return uncheckedGet(row).uncheckedGet(col);
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

    protected DoubleMatrix uncheckedSet(int row, int col, double value) {
        uncheckedGet(row).uncheckedSet(col, value);
        return this;
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
                val = DoubleVector.partial2(f, x, row, col, eps);
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

    public double trace() {
        double tr = 0;
        for (int row = 0; row < Math.min(rowsCount(), colsCount()); row++) tr += uncheckedGet(row, row);
        return tr;
    }

    /// later
    /// https://www.codeproject.com/Articles/1268576/Singular-Values-Decomposition-SVD-in-Cplusplus11-b

    double determinant() {
        DoubleMatrix copy = new DoubleMatrix(this);
        double det = 1.0;
        int row, col, pivot;
        for (row = 0; row < copy.rowsCount(); row++) {
            pivot = row;
            for (col = row + 1; col < copy.rowsCount(); col++) {
                if (Math.abs(copy.uncheckedGet(col, row)) > Math.abs(copy.uncheckedGet(pivot, row))) {
                    pivot = col;
                }
            }
            if (pivot != row) {
                var tmp = copy.uncheckedGet(pivot, col);
                copy.uncheckedSet(pivot, col, copy.uncheckedGet(row, col));
                copy.uncheckedSet(row, col, tmp);
                det *= -1.0;
            }
            if (Math.abs(copy.uncheckedGet(row, row)) < NumericCommon.NUMERIC_ACCURACY_HIGH) {
                return 0;
            }
            det *= copy.uncheckedGet(row, row);
            for (int j = row + 1; j < copy.rowsCount(); j++) {
                double factor = copy.uncheckedGet(j, row) / copy.uncheckedGet(row, row);
                for (int k = row + 1; k < copy.rowsCount(); k++) {
                    copy.uncheckedSet(j, k, copy.uncheckedGet(j, k) - factor * copy.uncheckedGet(row, k));
                }
            }
        }
        return det;
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

        int rank_a = DoubleMatrix.rank(a);

        DoubleMatrix ab = new DoubleMatrix(A);

        int rank_a_b = DoubleMatrix.rank(ab.addCol(b));

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
                System.out.println("mathUtils.Matrix is probably singular :: unable to solve A^-1 b = x");
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
                System.out.println("mathUtils.Matrix is probably singular :: unable to calculate invert matrix");
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

    private static void checkSizes(DoubleMatrix left, DoubleMatrix right) {
        if (left.rowsCount() != right.rowsCount()) throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
        if (left.colsCount() != right.rowsCount()) throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
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
        return new DoubleVector(map(left, row -> row.dot(right)));
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
        if (left.colsCount() != right.size()) throw new RuntimeException("unable to matrix and vector multiply");
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

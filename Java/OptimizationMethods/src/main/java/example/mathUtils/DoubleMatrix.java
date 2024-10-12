package example.mathUtils;


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

    ///////////////////////////////
    //      ADDITION INTERNAL    //
    ///////////////////////////////
    public DoubleMatrix add(DoubleMatrix other) {
        DoubleMatrixUtils.checkSizes(this, other);
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
        DoubleMatrixUtils.checkSizes(this, other);
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
    // public enjoy.mathUtils.Matrix mul(enjoy.mathUtils.Matrix other) ...

    ///////////////////////////////
    //     DIVISION INTERNAL     //
    ///////////////////////////////
    public DoubleMatrix div(double other) {
        return this.mul(1.0 / other);
    }
    // public enjoy.mathUtils.Matrix div(enjoy.mathUtils.Matrix other) ...

}

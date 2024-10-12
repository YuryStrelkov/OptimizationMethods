package example.labs;

import example.labs.enums.Sign;
import example.labs.enums.SimplexProblemType;
import example.mathUtils.*;

////////////////////
/// Lab. work #5 ///
////////////////////

public class Simplex {

    private int maxIterations = NumericCommon.ITERATIONS_COUNT_LOW;

    public int getMaxIterations() {
        return maxIterations;
    }

    public void setMaxIterations(int value) {
        maxIterations = NumericUtils.clamp(value, 10, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    private final TemplateVector<Sign> inequalities;

    // список индексов переменных которые войдут в целевую функию, модифицируя ее
    private final TemplateVector<Integer> virtualArgsIndices;

    // индексы естественных переменных
    private final TemplateVector<Integer> naturalArgsIndices;

    // список индексов текущих базисных переменных
    private final TemplateVector<Integer> basisArgsIndices;

    // Симплекс таблица
    private DoubleMatrix simplexTable;

    // матрица ограничений
    private final DoubleMatrix boundsMatrix;

    // вектор ограничений
    private final DoubleVector boundsVector;

    // вектор стоимостей
    private final DoubleVector pricesVector;

    /// режим поиска решения
    SimplexProblemType mode = SimplexProblemType.Max;

    public int naturalArgsCount() {
        return pricesVector.size();
    }

    public final DoubleMatrix boundsMatrix() {
        return boundsMatrix;
    }

    public final DoubleVector boundsVector() {
        return boundsVector;
    }

    public final DoubleVector pricesVector() {
        return pricesVector;
    }

    public final TemplateVector<Sign> inequalities() {
        return inequalities;
    }

    public final TemplateVector<Integer> basisArgsIndices() {
        return basisArgsIndices;
    }

    public final DoubleMatrix simplexTable() {
        return simplexTable;
    }

    public DoubleVector currentSimplexSolution(boolean onlyNaturalArgs) {
        DoubleVector solution = new DoubleVector(onlyNaturalArgs ? naturalArgsCount() : simplexTable.colsCount() - 1);

        for (int i = 0; i < basisArgsIndices.size(); i++) {
            if (basisArgsIndices.get(i) >= solution.size()) continue;

            solution.set(basisArgsIndices.get(i), simplexTable.get(i, simplexTable.colsCount() - 1));
        }
        return solution;
    }

    public DoubleVector currentSimplexSolution() {
        return currentSimplexSolution(false);
    }

    public boolean isTargetFuncModified() {
        return virtualArgsIndices.size() != 0;
    }

    /**
     * Проверяет оптимальность текущего опорного плана.
     * Исследуется положительность симплекс-разностей в последней строке СТ в диапазоне от 1:n-1.
     * Если целевая функция была модифицирована, то исследует две последних строки.
     * Если среди элементов от 1:n-1 в последней строке нет отрицательных, то проверяет
     * на неотрицательность только те элементы предпоследней строки, которые не являются искусственными.
     *
     * @return является ли план оптимальным
     */
    public boolean isPlanOptimal() {
        // Проверяем значения последней строки сиплекс-разностей
        // на положительность. Если все положительны, то план оптимален
        DoubleVector row = simplexTable.row(simplexTable.rowsCount() - 1);
        boolean optimal = true;
        for (int i = 0; i < (row.size() - 1); i++) {
            optimal = row.get(i) < 0;
            if (!optimal) break;
        }
        if (!isTargetFuncModified()) return optimal;
        // если мы модифицировали целевую функцию, то среди списка естественнхых
        // агументов проверям на положительнность предпослднюю строку симплекс-разностей
        DoubleVector row_ = simplexTable.row(simplexTable.rowsCount() - 2);
        for (int id : naturalArgsIndices) {
            optimal &= row_.get(id) < 0;
            if (!optimal) break;
        }
        return optimal;
    }

    /**
     * Определяет ведущий столбец. Среди элементов строки симплекс-разностей ищет максимальны по модулю
     * отрицательный элемент. Если целевая функция была модифицирована и среди последней строки нет отрицательных
     * элементов, то посик таковых будет продолжен среди только тех элементов предпоследней строки, которые не
     * являются искусственными.
     *
     * @return индекс ведущего столбца
     */
    private int getMainCol() {
        DoubleVector row = simplexTable.row(simplexTable.rowsCount() - 1);

        double delta = 0;

        int index = -1;

        for (int i = 0; i < row.size() - 1; i++) {
            if (row.get(i) >= delta) continue;
            delta = row.get(i);
            index = i;
        }

        if (!(isTargetFuncModified() && index == -1)) return index;

        row = simplexTable.row(simplexTable.rowsCount() - 2);

        for (int id : naturalArgsIndices) {
            if (row.get(id) >= delta) continue;
            delta = row.get(id);
            index = id;
        }

        return index;
    }

    /**
     * Определяет ведущую строку
     *
     * @param simplexCol ведущий столбец
     * @return индекс ведущей строки
     */
    int getMainRow(int simplexCol) {
        double delta = 1e12;

        int index = -1;

        double aik;

        int bIndex = simplexTable.colsCount() - 1;

        int rowsN = isTargetFuncModified() ? simplexTable.rowsCount() - 2 : simplexTable.rowsCount() - 1;

        for (int i = 0; i < rowsN; i++) {
            aik = simplexTable.get(i, simplexCol);
            if (aik < 0) continue;
            if (simplexTable.get(i, bIndex) / aik > delta) continue;
            delta = simplexTable.get(i, bIndex) / aik;
            index = i;
        }

        return index;
    }

    /**
     * строит виртуальный базисный вектор
     *
     * @param inequalityId   номер знака неравентсва
     * @param inequalitySign знак неравенства
     * @return массив индексов добавленных столбцов
     */
    private int[] buildVirtualBasisCol(int inequalityId, Sign inequalitySign) {
        if (inequalitySign == Sign.Equal) {
            simplexTable.addCol();
            simplexTable.set(inequalityId, simplexTable.colsCount() - 1, 1.0);
            return new int[]{simplexTable.colsCount() - 1, simplexTable.colsCount() - 1};
        }

        if (inequalitySign == Sign.More) {
            simplexTable.addCol();
            simplexTable.addCol();
            simplexTable.set(inequalityId, simplexTable.colsCount() - 2, -1.0);
            simplexTable.set(inequalityId, simplexTable.colsCount() - 1, 1.0);
            return new int[]{simplexTable.colsCount() - 2, simplexTable.colsCount() - 1};
        }
        simplexTable.addCol();
        simplexTable.set(inequalityId, simplexTable.colsCount() - 1, 1.0);
        return new int[]{simplexTable.colsCount() - 1, -1};
    }

    /**
     * Строит СМ таблицу для задачи вида:
     * Маирица системы ограниченй:
     * |u 0 0|
     * A = |0 v 0|
     * |0 0 w|
     * Вектор ограничений
     * |a|
     * b = |d|
     * |f|
     * с -коэффициенты целевой функции
     * f = (x,c)->extr
     * <p>
     * |u 0 0|   |x| <= |b|
     * |0 v 0| * |x| >= |f|
     * |0 0 w|   |x| =  |d|
     */
    private void buildSimplexTable() {
        simplexTable = new DoubleMatrix(boundsMatrix);
        naturalArgsIndices.clear(); // (индексы) x1, x2, x3 - те переменные, котоыре были в постановке задачи
        basisArgsIndices.clear();      // (индексы) xi, xi+1, ..., xn - те переменные, которые вошли в базис
        virtualArgsIndices.clear();       // (индексы) xi, xi+1, ..., xn - те переменные, которые модифицировали целевую функцию
        // Если среди вектора b есть отрицательные значения, то соответствующие строки
        // матрицы ограничений умножаем на мину один и меняем знак сравнения
        for (int row = 0; row < simplexTable.rowsCount(); row++) {
            if (boundsVector.get(row) >= 0) continue;
            inequalities.set(row, inequalities.get(row) == Sign.Less ? Sign.More : Sign.Less);
            boundsVector.set(row, boundsVector.get(row) * -1.0);
            simplexTable.row(row).mul(-1.0);
        }
        // построение списка индексов естественных переменных(те, что не искусственные)
        for (int i = 0; i < pricesVector.size(); i++) naturalArgsIndices.pushBack(i);
        // построение искуственного базиса
        int[] basisArgsInfo;

        for (int inequalityId = 0; inequalityId < inequalities.size(); inequalityId++) {
            basisArgsInfo = buildVirtualBasisCol(inequalityId, inequalities.get(inequalityId));

            naturalArgsIndices.pushBack(basisArgsInfo[0]);

            if (basisArgsInfo[1] != -1) {
                basisArgsIndices.pushBack(basisArgsInfo[1]);
                virtualArgsIndices.pushBack(basisArgsInfo[1]);
                continue;
            }
            basisArgsIndices.pushBack(basisArgsInfo[0]);
        }
        // добавим столбец ограницений
        simplexTable.addCol(boundsVector);
        // Построение симплекс разностей
        DoubleVector simD = new DoubleVector(simplexTable.colsCount());

        if (mode == SimplexProblemType.Max) {
            simD.applyEnumerate(pricesVector, (i, v) -> i < pricesVector.size() ? -v : 0.0);
        } else {
            simD.applyEnumerate(pricesVector, (i, v) -> i < pricesVector.size() ? v : 0.0);
        }
        simplexTable.addRow(simD);
        // Если целевая функуция не была модифицирована
        if (!isTargetFuncModified()) return;
        // Если всё же была...
        DoubleVector sDeltasAdd = new DoubleVector(simplexTable.colsCount());
        for (Integer fModArgId : virtualArgsIndices) sDeltasAdd.set(fModArgId, 1.0);
        simplexTable.addRow(sDeltasAdd);
    }

    private boolean excludeModArgs() {
        if (!isTargetFuncModified()) return false;

        int lastRowId = simplexTable.rowsCount() - 1;

        for (Integer fModArgId : virtualArgsIndices) {
            for (int row = 0; row < simplexTable.rowsCount(); row++) {
                if (simplexTable.get(row, fModArgId) == 0) continue;
                double arg = simplexTable.get(lastRowId, fModArgId) / simplexTable.get(row, fModArgId);
                simplexTable.row(lastRowId).sub(DoubleVectorUtils.mul(arg, simplexTable.row(row)));
                break;
            }
        }
        return true;
    }

    private boolean validateSolution() {
        double val = 0;

        int nRows = isTargetFuncModified() ? simplexTable.rowsCount() - 2 : simplexTable.rowsCount() - 1;

        int nCols = simplexTable.colsCount() - 1;

        for (int i = 0; i < basisArgsIndices.size(); i++) {
            if (basisArgsIndices.get(i) >= naturalArgsCount()) continue;
            val += simplexTable.get(i, nCols) * pricesVector.get(basisArgsIndices.get(i));
        }
        if (mode == SimplexProblemType.Max) {
            if (Math.abs(val - simplexTable.get(nRows, nCols)) < NumericCommon.NUMERIC_ACCURACY_MIDDLE) {
                if (isTargetFuncModified()) {
                    return (Math.abs(simplexTable.get(simplexTable.rowsCount() - 1, nCols)) < NumericCommon.NUMERIC_ACCURACY_MIDDLE);
                }
                return true;
            }
        }
        if (Math.abs(val + simplexTable.get(nRows, nCols)) < NumericCommon.NUMERIC_ACCURACY_MIDDLE) {
            if (isTargetFuncModified()) {
                return (Math.abs(simplexTable.get(simplexTable.rowsCount() - 1, nCols)) < NumericCommon.NUMERIC_ACCURACY_MIDDLE);
            }
            return true;
        }
        return false;
    }

    @Override
    public String toString() {
        if (simplexTable.rowsCount() == 0) return "";

        StringBuilder sb = new StringBuilder();

        sb.append(String.format("%-6s", ""));

        for (int col = 0; col < simplexTable.colsCount() - 1; col++) {
            sb.append(String.format("|%-12s", String.format(" x %s", (col + 1))));
        }
        sb.append(String.format("|%-12s", " b"));

        sb.append("\n");

        int nRow = -1;

        for (DoubleVector row : simplexTable.getRows()) {
            nRow++;

            if (isTargetFuncModified()) {
                if (nRow == simplexTable.rowsCount() - 2) {
                    sb.append(String.format("%-6s", " d0"));
                } else if (nRow == simplexTable.rowsCount() - 1) {
                    sb.append(String.format("%-6s", " d1"));
                } else {
                    sb.append(String.format("%-6s", String.format(" x %s", basisArgsIndices.get(nRow) + 1)));
                }
            } else {
                if (nRow == simplexTable.rowsCount() - 1) {
                    sb.append(String.format("%-6s", " d"));
                } else {
                    sb.append(String.format("%-6s", String.format(" x %s", basisArgsIndices.get(nRow) + 1)));
                }
            }
            for (int col = 0; col < row.size(); col++) {
                if (row.get(col) >= 0) {
                    sb.append(String.format("| %-11s", NumericUtils.toRationalStr(row.get(col))));
                    continue;
                }
                sb.append(String.format("|%-12s", NumericUtils.toRationalStr(row.get(col))));

            }
            sb.append("\n");
        }
        sb.append("\n");

        return sb.toString();
    }

    public DoubleVector solve(SimplexProblemType mode) {
        this.mode = mode;

        System.out.printf("enjoy.labs.Simplex problem type : %s\n", mode);

        buildSimplexTable();

        double aik;

        int mainRow;

        int mainCol;

        int iteration = 0;

        System.out.println("Start simplex table:");
        System.out.println(this);

        if (excludeModArgs()) {
            // второй этап, если задача должна решаться двух проходным(двух этапным) алгоритмом
            System.out.println("enjoy.labs.Simplex table after args exclude:");
            System.out.println(this);
        }

        while ((!isPlanOptimal()) || (getMaxIterations() == iteration)) {
            iteration++;

            mainCol = getMainCol();

            if (mainCol == -1) {
                break;
            }

            mainRow = getMainRow(mainCol);

            if (mainRow == -1) {
                // Невозможность определить ведущую строку свидейтельствует о том, что обрасть поиска неограничена
                System.out.println("Unable to get main row. enjoy.labs.Simplex is probably boundless...");
                return null;
            }

            basisArgsIndices.set(mainRow, mainCol);

            aik = simplexTable.get(mainRow, mainCol);

            final DoubleVector mainRowVector = simplexTable.row(mainRow).mul(1.0 / aik);

            for (int i = 0; i < simplexTable.rowsCount(); i++) {
                if (i == mainRow) continue;
                simplexTable.row(i).sub(DoubleVectorUtils.mul(simplexTable.get(i, mainCol), mainRowVector));
            }
            // solution = currentSimplexSolution();
            if (NumericCommon.SHOW_SIMPLEX_DEBUG_LOG) {
                System.out.printf("a_main{%s, %s} = %s\n", mainRow + 1, mainCol + 1, NumericUtils.toRationalStr(aik));
                System.out.println(this);
                System.out.printf("current solution: %s\n", NumericUtils.toRationalStr(currentSimplexSolution()));
            }
        }
        if (validateSolution()) {
            DoubleVector solution = currentSimplexSolution(true);
            // формирование ответа
            System.out.printf("solution: %s\n", NumericUtils.toRationalStr(solution));
            return solution;
        }
        System.out.println("enjoy.labs.Simplex is unresolvable");
        // значение целевой функции не равно ее значению от найденного плана
        return null;
    }

    public DoubleVector solve() {
        return solve(SimplexProblemType.Max);
    }

    public Simplex(DoubleMatrix a, DoubleVector c, TemplateVector<Sign> inequalities, DoubleVector b) {
        if (b.size() != inequalities.size()) {
            throw new RuntimeException("Error simplex creation :: b.size() != inequalities.size()");
        }

        if (a.rowsCount() != inequalities.size()) {
            throw new RuntimeException("Error simplex creation :: A.rows_number() != inequalities.size()");
        }

        if (a.colsCount() != c.size()) {
            throw new RuntimeException("Error simplex creation :: A.cols_number() != price coefficients.size()");
        }

        naturalArgsIndices = new TemplateVector<>();
        basisArgsIndices = new TemplateVector<>();
        virtualArgsIndices = new TemplateVector<>();

        boundsVector = new DoubleVector(b);
        boundsMatrix = new DoubleMatrix(a);
        pricesVector = new DoubleVector(c);
        this.inequalities = inequalities;
    }

    public Simplex(DoubleMatrix a, DoubleVector c, DoubleVector b) {
        if (b.size() != b.size()) {
            throw new RuntimeException("Error simplex creation :: b.size() != bounds coefficients.size()");
        }

        if (a.colsCount() != c.size()) {
            throw new RuntimeException("Error simplex creation :: A.cols_number() != price coefficients.size()");
        }

        inequalities = new TemplateVector<>(b.size());
        inequalities.fill(v -> Sign.Less);

        naturalArgsIndices = new TemplateVector<>();
        basisArgsIndices = new TemplateVector<>();
        virtualArgsIndices = new TemplateVector<>();

        boundsVector = new DoubleVector(b);
        boundsMatrix = new DoubleMatrix(a);
        pricesVector = new DoubleVector(c);
    }

}

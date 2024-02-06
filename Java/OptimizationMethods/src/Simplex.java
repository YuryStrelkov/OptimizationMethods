import mathUtils.NumericCommon;
import mathUtils.DoubleVector;
import mathUtils.DoubleMatrix;
import mathUtils.NumericUtils;

import java.util.ArrayList;

enum Sign {
    Equal,
    Less,
    More
}

enum SimplexProblemType {
    Min,
    Max,
}

////////////////////
/// Lab. work #5 ///
////////////////////

public class Simplex {

    private int maxIterations = NumericCommon.ITERATIONS_COUNT_LOW;

    public int getMaxIterations(){
        return maxIterations;
    }

    public void setMaxIterations(int value){
        maxIterations = NumericUtils.clamp(value, 10, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    private final ArrayList<Sign> inequalities;

    // список индексов переменных которые войдут в целевую функию, модифицируя ее
    private final ArrayList<Integer> fModArgs;

    // индексы естественных переменных
    private final ArrayList<Integer> naturalArgsIds;

    // список индексов текущих базисных переменных
    private final ArrayList<Integer> basisArgs;

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

    public int naturalArgsN() {
        return pricesVector.size();
    }

    public DoubleMatrix boundsMatrix() {
        return boundsMatrix;
    }

    public DoubleVector boundsCoefficients() {
        return boundsVector;
    }

    public DoubleVector pricesCoefficients() {
        return pricesVector;
    }

    public ArrayList<Sign> inequalities() {
        return inequalities;
    }

    public ArrayList<Integer> basisArguments() {
        return basisArgs;
    }

    public DoubleMatrix simplexTable() {
        return simplexTable;
    }

    public DoubleVector currentSimplexSolution(boolean onlyNaturalArgs) {
        DoubleVector solution = new DoubleVector(onlyNaturalArgs ? naturalArgsN() : simplexTable.cols() - 1);

        for (int i = 0; i < basisArgs.size(); i++) {
            if (basisArgs.get(i) >= solution.size()) continue;

            solution.set(basisArgs.get(i), simplexTable.get(i, simplexTable.cols() - 1));
        }
        return solution;
    }

    public DoubleVector currentSimplexSolution() {
        return currentSimplexSolution(false);
    }


    public boolean isTargetFuncModified() {
        return fModArgs.size() != 0;
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
        DoubleVector row = simplexTable.row(simplexTable.rows() - 1);
        boolean optimal = true;
        for (int i = 0; i < (row.size() - 1); i++) {
            optimal = row.get(i) < 0;
            if (!optimal) break;
        }
        if (!isTargetFuncModified()) return optimal;
        // если мы модифицировали целевую функцию, то среди списка естественнхых
        // агументов проверям на положительнность предпослднюю строку симплекс-разностей
        DoubleVector row_ = simplexTable.row(simplexTable.rows() - 2);
        for (int id : naturalArgsIds) {
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
        DoubleVector row = simplexTable.row(simplexTable.rows() - 1);

        double delta = 0;

        int index = -1;

        for (int i = 0; i < row.size() - 1; i++) {
            if (row.get(i) >= delta) continue;
            delta = row.get(i);
            index = i;
        }

        if (!(isTargetFuncModified() && index == -1)) return index;

        row = simplexTable.row(simplexTable.rows() - 2);

        for (int id : naturalArgsIds) {
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

        int bIndex = simplexTable.cols() - 1;

        int rowsN = isTargetFuncModified() ? simplexTable.rows() - 2 : simplexTable.rows() - 1;

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
            for (int row = 0; row < simplexTable.rows(); row++) {
                if (row == inequalityId) {
                    simplexTable.row(row).pushBack(1.0);
                    continue;
                }
                simplexTable.row(row).pushBack(0.0);
            }
            return new int[]{simplexTable.cols() - 1, simplexTable.cols() - 1};
        }

        if (inequalitySign == Sign.More) {
            for (int row = 0; row < simplexTable.rows(); row++) {
                if (row == inequalityId) {
                    simplexTable.row(row).pushBack(-1.0);
                    simplexTable.row(row).pushBack(1.0);
                    continue;
                }
                simplexTable.row(row).pushBack(0.0);
                simplexTable.row(row).pushBack(0.0);
            }
            return new int[]{simplexTable.cols() - 2, simplexTable.cols() - 1};
        }

        for (int row = 0; row < simplexTable.rows(); row++) {
            if (row == inequalityId) {
                simplexTable.row(row).pushBack(1.0);
                continue;
            }
            simplexTable.row(row).pushBack(0.0);
        }
        return new int[]{simplexTable.cols() - 1, -1};
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
        naturalArgsIds.clear(); // (индексы) x1, x2, x3 - те переменные, котоыре были в постановке задачи
        basisArgs.clear();      // (индексы) xi, xi+1, ..., xn - те переменные, которые вошли в базис
        fModArgs.clear();       // (индексы) xi, xi+1, ..., xn - те переменные, которые модифицировали целевую функцию

        // Если среди вектора b есть отрицательные значения, то соответствующие строки
        // матрицы ограничений умножаем на мину один и меняем знак сравнения
        for (int row = 0; row < simplexTable.rows(); row++) {
            if (boundsVector.get(row) >= 0) continue;
            inequalities.set(row, inequalities.get(row) == Sign.Less ? Sign.More : Sign.Less);
            boundsVector.set(row, boundsVector.get(row) * -1.0);
            simplexTable.row(row).mul(-1.0);
        }

        // построение списка индексов естественных переменных(те, что не искусственные)
        for (int i = 0; i < pricesVector.size(); i++) naturalArgsIds.add(i);

        // построение искуственного базиса
        int[] basisArgsInfo;

        for (int inequalityId = 0; inequalityId < inequalities.size(); inequalityId++) {
            basisArgsInfo = buildVirtualBasisCol(inequalityId, inequalities.get(inequalityId));

            naturalArgsIds.add(basisArgsInfo[0]);

            if (basisArgsInfo[1] != -1) {
                basisArgs.add(basisArgsInfo[1]);
                fModArgs.add(basisArgsInfo[1]);
                continue;
            }
            basisArgs.add(basisArgsInfo[0]);
        }

        // добавим столбец ограницений
        for (int row = 0; row < simplexTable.rows(); row++) simplexTable.row(row).pushBack(boundsVector.get(row));

        // Построение симплекс разностей
        DoubleVector simD = new DoubleVector(simplexTable.cols());

        if (mode == SimplexProblemType.Max) {
            // simD.applyEnumerate(pricesVector, (i, v)-> i < pricesVector.size() ? -v : 0.0);
            for (int j = 0; j < simD.size(); j++) simD.set(j, j < pricesVector.size() ? -pricesVector.get(j) : 0.0);
        } else {
            // simD.applyEnumerate(pricesVector, (i, v)-> i < pricesVector.size() ? v : 0.0);
            for (int j = 0; j < simD.size(); j++) simD.set(j, j < pricesVector.size() ? pricesVector.get(j) : 0.0);
        }

        simplexTable.addRow(simD);

        // Если целевая функуция не была модифицирована
        if (!isTargetFuncModified()) return;

        // Если всё же была...
        DoubleVector sDeltasAdd = new DoubleVector(simplexTable.cols());

        for (Integer fModArgId : fModArgs) sDeltasAdd.set(fModArgId, 1.0);

        simplexTable.addRow(sDeltasAdd);
    }

    private boolean excludeModArgs() {
        if (!isTargetFuncModified()) return false;

        int lastRowId = simplexTable.rows() - 1;

        for (Integer fModArgId : fModArgs) {
            for (int row = 0; row < simplexTable.rows(); row++) {
                if (simplexTable.get(row, fModArgId) == 0) continue;
                double arg = simplexTable.get(lastRowId, fModArgId) / simplexTable.get(row, fModArgId);
                simplexTable.row(lastRowId).sub(DoubleVector.mul(arg, simplexTable.row(row)));
                break;
            }
        }
        return true;
    }

    private boolean validateSolution() {
        double val = 0;

        int nRows = isTargetFuncModified() ? simplexTable.rows() - 2 : simplexTable.rows() - 1;

        int nCols = simplexTable.cols() - 1;

        for (int i = 0; i < basisArgs.size(); i++) {
            if (basisArgs.get(i) >= naturalArgsN()) continue;
            val += simplexTable.get(i, nCols) * pricesVector.get(basisArgs.get(i));
        }
        if (mode == SimplexProblemType.Max) {
            if (Math.abs(val - simplexTable.get(nRows, nCols)) < NumericCommon.NUMERIC_ACCURACY_MIDDLE) {
                if (isTargetFuncModified()) {
                    return (Math.abs(simplexTable.get(simplexTable.rows() - 1, simplexTable.cols() - 1)) < NumericCommon.NUMERIC_ACCURACY_MIDDLE);
                }
                return true;
            }
        }
        if (Math.abs(val + simplexTable.get(nRows, nCols)) < NumericCommon.NUMERIC_ACCURACY_MIDDLE) {
            if (isTargetFuncModified()) {
                return (Math.abs(simplexTable.get(simplexTable.rows() - 1, simplexTable.cols() - 1)) < NumericCommon.NUMERIC_ACCURACY_MIDDLE);
            }
            return true;
        }
        return false;
    }

    @Override
    public String toString() {
        if (simplexTable.rows() == 0) return "";

        StringBuilder sb = new StringBuilder();

        sb.append(String.format("%-6s", ""));

        for (int col = 0; col < simplexTable.cols() - 1; col++) {
            sb.append(String.format("|%-12s", String.format(" x %s", (col + 1))));
        }
        sb.append(String.format("|%-12s", " b"));

        sb.append("\n");

        int nRow = -1;

        for (DoubleVector row : simplexTable.getRows()) {
            nRow++;

            if (isTargetFuncModified()) {
                if (nRow == simplexTable.rows() - 2) {
                    sb.append(String.format("%-6s", " d0"));
                } else if (nRow == simplexTable.rows() - 1) {
                    sb.append(String.format("%-6s", " d1"));
                } else {
                    sb.append(String.format("%-6s", String.format(" x %s", basisArgs.get(nRow) + 1)));
                }
            } else {
                if (nRow == simplexTable.rows() - 1) {
                    sb.append(String.format("%-6s", " d"));
                } else {
                    sb.append(String.format("%-6s", String.format(" x %s", basisArgs.get(nRow) + 1)));
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

        System.out.printf("Simplex problem type : %s\n", mode);

        buildSimplexTable();

        double aik;

        int mainRow;

        int mainCol;

        int iteration = 0;

        System.out.println("Start simplex table:");
        System.out.println(this);

        if (excludeModArgs()) {
            // второй этап, если задача должна решаться двух проходным(двух этапным) алгоритмом
            System.out.println("Simplex table after args exclude:");
            System.out.println(this);
        }

        while ((!isPlanOptimal()) || (getMaxIterations() == iteration)){
            iteration++;

            mainCol = getMainCol();

            if (mainCol == -1) {
                break;
            }

            mainRow = getMainRow(mainCol);

            if (mainRow == -1) {
                // Невозможность определить ведущую строку свидейтельствует о том, что обрасть поиска неограничена
                System.out.println("Unable to get main row. Simplex is probably boundless...");
                return null;
            }

            basisArgs.set(mainRow, mainCol);

            aik = simplexTable.get(mainRow, mainCol);

            simplexTable.row(mainRow).mul(1.0 / aik);

            for (int i = 0; i < simplexTable.rows(); i++) {
                if (i == mainRow) continue;
                simplexTable.row(i).sub(DoubleVector.mul(simplexTable.get(i, mainCol), simplexTable.row(mainRow)));
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
        System.out.println("Simplex is unresolvable");
        // значение целевой функции не равно ее значению от найденного плана
        return null;
    }

    public DoubleVector solve() {
        return solve(SimplexProblemType.Max);
    }

    public Simplex(DoubleMatrix a, DoubleVector c, ArrayList<Sign> inequalities, DoubleVector b) {
        if (b.size() != inequalities.size()) {
            throw new RuntimeException("Error simplex creation :: b.size() != inequalities.size()");
        }

        if (a.rows() != inequalities.size()) {
            throw new RuntimeException("Error simplex creation :: A.rows_number() != inequalities.size()");
        }

        if (a.cols() != c.size()) {
            throw new RuntimeException("Error simplex creation :: A.cols_number() != price coefficients.size()");
        }

        naturalArgsIds = new ArrayList<>();
        basisArgs = new ArrayList<>();
        fModArgs = new ArrayList<>();

        boundsVector = new DoubleVector(b);
        boundsMatrix = new DoubleMatrix(a);
        pricesVector = new DoubleVector(c);
        this.inequalities = inequalities;
    }

    public Simplex(DoubleMatrix a, DoubleVector c, DoubleVector b) {
        if (b.size() != b.size()) {
            throw new RuntimeException("Error simplex creation :: b.size() != bounds coefficients.size()");
        }

        if (a.cols() != c.size()) {
            throw new RuntimeException("Error simplex creation :: A.cols_number() != price coefficients.size()");
        }

        inequalities = new ArrayList<>();

        for (int i = 0; i < b.size(); i++) inequalities.add(Sign.Less);

        naturalArgsIds = new ArrayList<>();
        basisArgs = new ArrayList<>();
        fModArgs = new ArrayList<>();

        boundsVector = new DoubleVector(b);
        boundsMatrix = new DoubleMatrix(a);
        pricesVector = new DoubleVector(c);
    }

}

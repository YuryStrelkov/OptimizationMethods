import java.util.ArrayList;

final class Pair <FirstItemType,SecondItemType>
{
   public FirstItemType item1;
   public SecondItemType item2;
   public Pair(FirstItemType i1, SecondItemType i2)
   {
       item1 = i1;
       item2 = i2;
   }
}
enum Sign
{
    Equal,
    Less,
    More
};

enum SimplexProblemType
{
    Min,
    Max,
}

////////////////////
/// Lab. work #5 ///
////////////////////

public class Simplex
{
    public  static  boolean showSimplexDebugLog = false;

    private ArrayList<Sign> ineqs;

    // список индексов переменных которые войдут в целевую функию, модифицируя ее
    private ArrayList<Integer> f_mod_args;

    // индексы естественных переменных
    private  ArrayList<Integer> natural_args_ids;

    //индексы переменных, не являющихся искусственными
    private  ArrayList<Integer> artificial_args_ids;

    // список индексов текущих базисных переменных
    private  ArrayList<Integer> basis_args;

    // Симплекс таблица
    private  Matrix symplex_t;

    // матрица ограничений
    private Matrix bounds_m;

    // вектор ограничений
    private Vector bounds_v;

    // вектор стоимостей
    private Vector prices_v;

    /// режим поиска решения
    SimplexProblemType mode = SimplexProblemType.Max;

    public int naturalArgsN()
    {
        return prices_v.size();
    }

    public Matrix boundsMatrix()
    {
        return bounds_m;
    }

    public Vector boundsCoeffs()
    {
        return bounds_v;
    }

    public Vector pricesCoeffs()
    {
        return prices_v;
    }

    public ArrayList<Sign> inequations()
    {
        return ineqs;
    }

    public ArrayList<Integer> basisArgsuments()
    {
        return basis_args;
    }

    public Matrix symplexTable()
    {
        return symplex_t;
    }

    public Vector currentSymplexSolution(boolean only_natural_args)
    {
        Vector solution = new Vector(only_natural_args ? naturalArgsN() : symplex_t.cols() - 1);

        for (int i = 0; i < basis_args.size(); i++)
        {
            if (basis_args.get(i) >= solution.size())
            {
                continue;
            }

            solution.set(basis_args.get(i), symplex_t.get(i,symplex_t.cols() - 1));
        }
        return solution;
    }

    public Vector currentSymplexSolution()
    {
        return  currentSymplexSolution(false);
    }


    public boolean isTargetFuncModified()
    {
        return f_mod_args.size() != 0;
    }

    /**
     * Проверяет оптимальность текущего опорного плана.
     * Исследуется положительность симплекс-разностей в последней строке СТ в диапазоне от 1:n-1.
     * Если целевая функция была модифицирована, то исследует две последних строки.
     * Если среди элементов от 1:n-1 в последней строке нет отрицательных, то проверяет
     * на неотрицательность только те элементы предпоследней строки, которые не являются искусственными.
     * @return
     */
    public boolean isPlanOptimal()
    {
        // Проверяем значения последней строки сиплекс-разностей
        // на положительность. Если все положительны, то план оптимален

        Vector row = symplex_t.row(symplex_t.rows() - 1);

        boolean opt = true;

        for (int i = 0; i < row.size() - 1; i++)
        {
            if (row.get(i) < 0)
            {
                opt = false;
                break;
            }
        }

        // если мы модифицировали целевую функцию, то среди списка естественнхых
        // агументов проверям на положительнность предпослднюю строку симплекс-разностей

        if (isTargetFuncModified())
        {
            if (!opt)
            {
                return opt;
            }
            Vector row_ = symplex_t.row(symplex_t.rows() - 2);

            for (int id : natural_args_ids)
            {
                if (row_.get(id) < 0)
                {
                    opt = false;
                    break;
                }
            }
        }

        return opt;
    }

    /**
     * Определяет ведущий столбец. Среди элементов строки симплекс-разностей ищет максимальны по модулю
     * отрицательный элемент. Если целевая функция была модифицирована и среди последней строки нет отрицательных
     * элементов, то посик таковых будет продолжен среди только тех элементов предпоследней строки, которые не
     * являются искусственными.
     * @return
     */
    private int getMainCol()
    {
        Vector row = symplex_t.row(symplex_t.rows() - 1);

        double delta = 0;

        int index = -1;

        for (int i = 0; i < row.size() - 1; i++)
        {
            if (row.get(i) >= delta)
            {
                continue;
            }
            delta = row.get(i);
            index = i;
        }

        if (isTargetFuncModified() && index == -1)
        {
            Vector row_add = symplex_t.row(symplex_t.rows() - 2);

            for (int id : natural_args_ids)
            {
                if (row_add.get(id) >= delta)
                {
                    continue;
                }
                delta = row_add.get(id);
                index = id;
            }
        }
        return index;
    }

    /**
     * Определяет ведущую строку
     * @param symplex_col ведущий столбец
     * @return
     */
    int getMainRow(int symplex_col)
    {
        double delta = 1e12;

        int index = -1;

        double a_ik;

        int b_index = symplex_t.cols() - 1;

        int rows_n = isTargetFuncModified() ? symplex_t.rows() - 2 : symplex_t.rows() - 1;

        for (int i = 0; i < rows_n; i++)
        {
            a_ik = symplex_t.get(i, symplex_col);

            if (a_ik < 0)
            {
                continue;
            }
            if (symplex_t.get(i,b_index) / a_ik > delta)
            {
                continue;
            }
            delta = symplex_t.get(i,b_index) / a_ik;
            index = i;
        }

        return index;
    }

    /**
     * строит виртуальный базисный вектор
     * @param ineq_id
     * @param _ineq
     * @return
     */
    private int[] buildVirtualBasisCol(int ineq_id, Sign _ineq)//, ref int col_index, ref int col_index_aditional)
    {
        if (_ineq == Sign.Equal)
        {
            for (int row = 0; row < symplex_t.rows(); row++)
            {
                if (row == ineq_id)
                {
                    symplex_t.row(row).pushBack(1.0);
                    continue;
                }
                symplex_t.row(row).pushBack(0.0);
            }

            return new int[]{ symplex_t.cols() - 1, symplex_t.cols() - 1};
        }

        if (_ineq == Sign.More)
        {
            for (int row = 0; row < symplex_t.rows(); row++)
            {
                if (row == ineq_id)
                {
                    symplex_t.row(row).pushBack(-1.0);

                    symplex_t.row(row).pushBack(1.0);

                    continue;
                }

                symplex_t.row(row).pushBack(0.0);

                symplex_t.row(row).pushBack(0.0);
            }

            return new int[]{ symplex_t.cols() - 2, symplex_t.cols() - 1};
        }

        for (int row = 0; row < symplex_t.rows(); row++)
        {
            if (row == ineq_id)
            {
                symplex_t.row(row).pushBack(1.0);
                continue;
            }
            symplex_t.row(row).pushBack(0.0);
        }

        return new int[]{ symplex_t.cols() - 1, - 1};
    }

    /**
     *  Строит СМ таблицу для задачи вида:
     *  Маирица системы ограниченй:
     *  		|u 0 0|
     *      A = |0 v 0|
     *    		|0 0 w|
     *  Вектор ограничений
     *      |a|
     *  b = |d|
     *   	|f|
     *  с -коэффициенты целевой функции
     *  f = (x,c)->extr
     *
     *  |u 0 0|   |x| <= |b|
     *  |0 v 0| * |x| >= |f|
     *  |0 0 w|   |x| =  |d|
     *
     */
    private void buildSymplexTable()
    {
        symplex_t = new Matrix(bounds_m);
        natural_args_ids.clear();
        basis_args.clear();
        f_mod_args.clear();
        artificial_args_ids.clear();

        // Если среди вектора b есть отрицательные значения, то соответствующие строки
        // матрицы ограничений умножаем на мину один и меняем знак сравнения

        for (int row = 0; row < symplex_t.rows(); row++)
        {
            if (bounds_v.get(row) >= 0)
            {
                continue;
            }

            ineqs.set(row, ineqs.get(row) == Sign.Less ? Sign.More : Sign.Less);// = ineqs[row] == Sign.Less ? Sign.More : Sign.Less;

            bounds_v.set(row, bounds_v.get(row) * -1.0);//bounds_v[row] *= -1;

            symplex_t.row(row).mul(-1.0);/// = symplex_t[row] * (-1.0);
        }


        for (int i = 0; i < prices_v.size(); i++)
        {
            natural_args_ids.add(i);
        }

        // построение искуственного базиса
        int[] basis_args_info;

        for (int ineq_id = 0; ineq_id < ineqs.size(); ineq_id++)
        {
            basis_args_info = buildVirtualBasisCol(ineq_id, ineqs.get(ineq_id));//, ref basis_arg_id, ref basis_arg_id_add);

            natural_args_ids.add(basis_args_info[0]);

            if (basis_args_info[1] != -1)
            {
                basis_args.add(basis_args_info[1]);
                f_mod_args.add(basis_args_info[1]);
                artificial_args_ids.add(basis_args_info[1]);
                continue;
            }

            basis_args.add(basis_args_info[0]);
        }

        // добавим столбец ограницений

        for (int row = 0; row < symplex_t.rows(); row++)
        {
            symplex_t.row(row).pushBack(bounds_v.get(row));
        }

        // Построение симплекс разностей

        Vector s_deltas = new Vector(symplex_t.cols());

        if (mode == SimplexProblemType.Max)
        {
            for (int j = 0; j < s_deltas.size(); j++)
            {
                s_deltas.set(j, j < prices_v.size() ? -prices_v.get(j) : 0.0);
            }
        }
        else
        {
            for (int j = 0; j < s_deltas.size(); j++)
            {
                s_deltas.set(j,  j < prices_v.size() ? prices_v.get(j) : 0.0);
            }
        }

        symplex_t.addRow(s_deltas);

        // Если целевая функуция не была модифицирована

        if (!isTargetFuncModified())
        {
            return;
        }

        // Если всё же была...

        Vector s_deltas_add = new Vector(symplex_t.cols());

        for (int j = 0; j < f_mod_args.size(); j++)
        {
            s_deltas_add.set(f_mod_args.get(j),1.0);// = 1.0;
        }

        symplex_t.addRow(s_deltas_add);
    }

    private boolean excludeModArgs()
    {
        if (!isTargetFuncModified())
        {
            return false;
        }

        int last_row_id = symplex_t.rows() - 1;

        for (int i = 0; i < f_mod_args.size(); i++)
        {
            for (int row = 0; row < symplex_t.rows(); row++)
            {

                if (symplex_t.get(row,f_mod_args.get(i))!= 0)
                {
                    double arg = symplex_t.get(last_row_id,f_mod_args.get(i)) / symplex_t.get(row,f_mod_args.get(i)) ;// symplex_t[row][f_mod_args[i]];

                    symplex_t.row(last_row_id).sub(Vector.mul(arg,symplex_t.row(row)));/// = symplex_t[last_row_id] - arg * symplex_t[row];

                    break;
                }
            }
        }
        return true;
    }

    private boolean validateSolution()
    {
        double val = 0;

        int n_rows = isTargetFuncModified() ? symplex_t.rows() - 2 : symplex_t.rows() - 1;

        int n_cols = symplex_t.cols() - 1;

        for (int i = 0; i < basis_args.size(); i++)
        {
            if (basis_args.get(i) < naturalArgsN())
            {
                val += symplex_t.get(i,n_cols) * prices_v.get(basis_args.get(i));//symplex_t[i][n_cols] * prices_v[basis_args[i]];
            }
        }
        if (mode == SimplexProblemType.Max)
        {
            if (Math.abs(val - symplex_t.get(n_rows, n_cols)) < 1e-5)
            {
                if (isTargetFuncModified())
                {
                    return (Math.abs(symplex_t.get(symplex_t.rows() - 1, symplex_t.cols() - 1)) < 1e-5);
                }

                return true;
            }
        }
        if (Math.abs(val + symplex_t.get(n_rows, n_cols)) < 1e-5)
        {
            if (isTargetFuncModified())
            {
                return (Math.abs(symplex_t.get(symplex_t.rows() - 1, symplex_t.cols() - 1)) < 1e-5);
            }

            return true;
        }
        return false;
    }

    public  String symplexToString()
    {
        if (symplex_t.rows() == 0)
        {
            return "";
        }

        StringBuilder sb = new StringBuilder();

        int i = 0;

        sb.append(String.format("%-6s", " "));

        for (; i < symplex_t.cols() - 1; i++)
        {
            sb.append(String.format("|%-12s", " x " + String.valueOf((i + 1))));
        }
        sb.append(String.format("|%-12s", " b"));

        sb.append("\n");

        int n_row = -1;

        for(Vector row : symplex_t.getRows())
        {
            n_row++;

            if (isTargetFuncModified())
            {
                if (n_row == symplex_t.rows() - 2)
                {
                    sb.append(String.format("%-6s"," d0"));
                }
                else if (n_row == symplex_t.rows() - 1)
                {
                    sb.append(String.format("%-6s"," d1"));
                }
                else
                {
                    sb.append (String.format("%-6s", " x " + String.valueOf(basis_args.get(n_row) + 1)));
                }
            }
            else
            {
                if (n_row == symplex_t.rows() - 1)
                {
                    sb.append(String.format("%-6s"," d"));
                }
                else
                {
                    sb.append (String.format("%-6s", " x " + String.valueOf(basis_args.get(n_row) + 1)));
                }
            }

            for (int col = 0; col < row.size(); col++)
            {
                if (row.get(col) >= 0)
                {
                    sb.append(String.format("|%-12s", " " + NumericUtils.toRationalStr(row.get(col))));
                    continue;
                }
                sb.append(String.format("|%-12s", NumericUtils.toRationalStr(row.get(col))));

            }
            sb.append("\n");
        }
        sb.append("\n");

        return sb.toString();
    }

    public Vector solve(SimplexProblemType mode) /// = SymplexProblemType.Max)
    {
        this.mode = mode;

        System.out.println("Symplex problem type : " + mode.toString() + "\n");

        Vector solution = new Vector(naturalArgsN());

        buildSymplexTable();

        double a_ik;

        int main_row;

        int main_col;

        System.out.println("Start symplex table:");
        System.out.println(symplexToString());

        if (excludeModArgs())
        {
            // второй этап, если задача должна решаться двух проходным(двух этапным) алгоритмом
            System.out.println("Symplex table after args exclude:");
            System.out.println(symplexToString());
        }

        while (!isPlanOptimal())
        {
            main_col = getMainCol();

            if (main_col == -1)
            {
                break;
            }

            main_row = getMainRow(main_col);

            if (main_row == -1)
            {
                // Невозможность определить ведущую строку свидейтельствует о том, что обрасть поиска неограничена
                System.out.println("Unable to get main row. Symplex is probably boundless...");
                return null;
            }

            basis_args.set(main_row, main_col);

            a_ik = symplex_t.get(main_row, main_col);

            symplex_t.row(main_row).mul(1.0 / a_ik);/// = symplex_t[main_row] * (1.0 / a_ik);

            for (int i = 0; i < symplex_t.rows(); i++)
            {
                if (i == main_row)
                {
                    continue;
                }
                symplex_t.row(i).sub(Vector.mul(symplex_t.get(i,main_col), symplex_t.row(main_row)));//  symplex_t[i][main_col] * symplex_t[main_row];
            }
            solution = currentSymplexSolution();
            if(showSimplexDebugLog)
            {
                System.out.println("a_main{"+ String.valueOf((main_row + 1))+", "+ String.valueOf((main_col + 1)) + "} = " + NumericUtils.toRationalStr(a_ik) + "\n");
                System.out.println(symplexToString());
                System.out.println("current solution: " + NumericUtils.toRationalStr(currentSymplexSolution()));
            }
        }
        if (validateSolution())
        {
            solution = currentSymplexSolution(true);
            // формирование ответа
            System.out.println("solution: " + NumericUtils.toRationalStr(solution));
            return solution;
        }
        System.out.println("Symplex is unresolvable");
        // значение целевой функции не равно ее значению от найденного плана
        return null;
    }

    public Vector solve()
    {
        return solve(SimplexProblemType.Max);
    }

    public Simplex(Matrix a, Vector c, ArrayList<Sign> _ineq, Vector b)
    {
        if (b.size() != _ineq.size())
        {
            throw new RuntimeException("Error symplex creation :: b.size() != inequation.size()");
        }

        if (a.rows() != _ineq.size())
        {
            throw new RuntimeException("Error symplex creation :: A.rows_number() != inequation.size()");
        }

        if (a.cols() != c.size())
        {
            throw new RuntimeException("Error symplex creation :: A.cols_number() != price_coeffs.size()");
        }

        natural_args_ids    = new ArrayList<Integer>();
        basis_args          = new ArrayList<Integer>();
        f_mod_args          = new ArrayList<Integer>();
        artificial_args_ids = new ArrayList<Integer>();

        bounds_v = b;

        bounds_m = a;

        prices_v = c;

        ineqs    = _ineq;
    }

    public Simplex(Matrix a, Vector c, Vector b)
    {
        if (b.size() != b.size())
        {
            throw new RuntimeException("Error symplex creation :: b.size() != bouns_coeffs.size()");
        }

        if (a.cols() != c.size())
        {
            throw new RuntimeException("Error symplex creation :: A.cols_number() != price_coeffs.size()");
        }

        ineqs = new ArrayList<Sign>();

        for (int i = 0; i < b.size(); i++)
        {
            ineqs.add(Sign.Less);
        }

        natural_args_ids    = new ArrayList<Integer>();
        basis_args          = new ArrayList<Integer>();
        f_mod_args          = new ArrayList<Integer>();
        artificial_args_ids = new ArrayList<Integer>();

        bounds_v = b;

        bounds_m = a;

        prices_v = c;
    }

}

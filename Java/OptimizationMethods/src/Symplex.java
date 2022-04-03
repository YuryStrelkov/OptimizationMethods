import java.util.ArrayList;

final class Pair<FisrtItemType,SecondItemType>
{
   public FisrtItemType  item1;
   public SecondItemType item2;
   public Pair(FisrtItemType i1,SecondItemType i2)
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
 enum SolutionType
{
    Single,
    Infinite,
    None
}
enum SymplexProblemType
{
    Min,
    Max,
}
public class Symplex
{
    private static String toRationalStr(double value, boolean fullRational)
    {
        int[] number =  NumericUtils.DecimalToRational(value);
        if (number[1] == 0)
        {
            return String.valueOf(number[0]);
        }
        if (number[0] == 0)
        {
            return String.valueOf(number[1]) + "/" + String.valueOf(number[2]);
        }

        if (fullRational)
        {
            return String.valueOf((number[1] + Math.abs(number[0]) * number[2]) * (number[0] >= 0 ? 1 : -1)) + "/" + String.valueOf(number[2]);
        }
        return String.valueOf(number[0]) + " " + String.valueOf(number[1]) + "/" + String.valueOf(number[2]);
    }
    private static String toRationalStr(double value)
    {
       return toRationalStr(value, true);
    }
    private static String toRationalStr(Vector value, boolean fullRational)
    {
        String str = "{ ";
        for (int i = 0; i < value.size() - 1; i++)
        {
            str += toRationalStr(value.get(i), fullRational);
            str += ", ";
        }
        str += toRationalStr(value.get(value.size() - 1), fullRational);

        str += " }";
        return str;
    }
    private static String toRationalStr(Vector value)
    {
        return toRationalStr( value,  true);
    }

    public static String symplexToString(Matrix table, ArrayList<Integer> basis)
    {
        if (table.rows() == 0)
        {
            return "";
        }

        StringBuilder sb = new StringBuilder();

        int i = 0;

        sb.append(String.format("%-6s", " "));

        for (; i < table.cols() - 1; i++)
        {
            sb.append(String.format("|%-12s", " x " + String.valueOf((i + 1))));
        }
        sb.append(String.format("|%-12s", " b"));

        sb.append("\n");

        int n_row = -1;

        for(Vector row : table.rows)
        {
            n_row++;

            if (n_row == table.rows() - 1)
            {
               ;
                sb.append(String.format("%-6s","d"));
            }
            else
            {
                sb.append (String.format("%-6s", " x " + String.valueOf(basis.get(n_row) + 1)));
            }

            for (int col = 0; col < row.size(); col++)
            {
                if (row.get(col) >= 0)
                {
                    sb.append(String.format("|%-12s", " " + toRationalStr(row.get(col))));
                    continue;
                }
                sb.append(String.format("|%-12s", toRationalStr(row.get(col))));

            }
            sb.append("\n");
        }
        sb.append("\n");

        return sb.toString();
    }

    /// <summary>
    /// Проверяет совместность СЛАУ вида Ax = b. Используется теорема Кронекера-Капелли
    /// </summary>
    /// <param name="A"></param>
    /// <param name="b"></param>
    /// <returns>0 - нет решений, 1 - одно решение, 2 - бесконечное множествое решений</returns>
    public static SolutionType checkSystem(Matrix A, Vector b)throws Exception
    {
        Matrix a = new Matrix(A);

        int rank_a = Matrix.rank(a);

        Matrix ab = new Matrix(A);

        int rank_a_b = Matrix.rank(ab.addCol(b));

        System.out.println("rank ( A ) "+rank_a+"\n");

        System.out.println("rank (A|b) "+rank_a_b+"\n");
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
        throw new Exception("error :: check_system");
    }

    /// <summary>
    /// Проверяет оптимальность плана в соответсвии с тем типом экстремума, которыей требуется найти.
    /// Провеяются элменты от 1:n-1 полсдней строки СМ таблицы
    /// </summary>
    /// <param name="A">СМ таблицa</param>
    /// <param name="mode"></param>
    /// <returns></returns>
    public static boolean isPlanOptimal(Matrix A, SymplexProblemType mode)
    {
        Vector deltas = A.rows.get(A.rows() - 1);

        if (mode == SymplexProblemType.Max)
        {
            for (int i = 0; i < deltas.size() - 1; i++)
            {
                if (deltas.get(i) < 0)
                {
                    return false;
                }
            }
            return true;
        }

        for (int i = 0; i < deltas.size() - 1; i++)
        {
            if (deltas.get(i) > 0)
            {
                return false;
            }
        }
        return true;
    }
    /// <summary>
    /// Определяет ведущий столбец в соответсвии с тем типом экстремума, который требуется найти.
    /// Исследуются элменты от 1:n-1 полсдней строки СМ таблицы
    /// </summary>
    /// <param name="A"></param>
    /// <returns></returns>
    public static int getMainCol(Matrix A, SymplexProblemType mode)
    {
        double delta = 0;

        int index = -1;

        Vector c = A.getRows().get(A.rows() - 1);

        if (SymplexProblemType.Min == mode)
        {
            for (int i = 0; i < c.size() - 1; i++)
            {
                if (c.get(i) <= 0)
                {
                    continue;
                }
                if (c.get(i) < delta)
                {
                    continue;
                }

                delta = c.get(i);

                index = i;
            }
            return index;
        }

        for (int i = 0; i < c.size() - 1; i++)
        {
            if (c.get(i) >= 0)
            {
                continue;
            }
            if (Math.abs(c.get(i)) < delta)
            {
                continue;
            }

            delta = Math.abs(c.get(i));

            index = i;
        }

        return index;
    }

    /// <summary>
    /// Определяет ведущую строку
    /// </summary>
    /// <param name="symplex_col">ведущий столбец</param>
    /// <param name="A">СМ таблица</param>
    /// <returns></returns>
    /// <summary>

    public static int getMainRow(int symplex_col, Matrix A)
    {
        double delta = 1e12;

        int index = -1;

        double a_ik;

        int b_index = A.cols() - 1;

        for (int i = 0; i < A.rows() - 1; i++)
        {
            a_ik = A.get(i,symplex_col);// [i][symplex_col];

            if (a_ik < 0)
            {
                continue;
            }
            if (A.get(i,b_index) / a_ik > delta)
            {
                continue;
            }
            delta = A.get(i,b_index) / a_ik;
            index = i;
        }
        return index;
    }

    /// Выводит текущее решение СМ таблицы для не искусственных переменных
    /// </summary>
    /// <param name="A">СМ таблица</param>
    /// <param name="basis">список базисных параметров</param>
    /// <param name="n_agrs">количество исходных переменных</param>
    /// <returns></returns>
    public static Vector currentSymplexSolution(Matrix A, ArrayList<Integer> basis, int n_agrs)
    {
        Vector solution = new Vector(n_agrs);
        for (int i = 0; i < basis.size(); i++)
        {
            if (basis.get(i) >= n_agrs)
            {
                continue;
            }
            solution.set(basis.get(i),A .get(i,A.cols()-1));//[i][A[0].Size - 1]);
        }
        return solution;
    }

    /// <summary>
    /// Строит СМ таблицу для задачи вида:
    /// Маирица системы ограниченй:
    ///		|u 0 0|
    /// A = |0 v 0|
    ///		|0 0 w|
    /// Вектор ограничений
    ///		|a|
    /// b = |d|
    ///		|f|
    /// с -коэффициенты целевой функции
    /// f = (x,c)->extr
    ///	|u 0 0|   |x| <= |b|
    /// |0 v 0| * |x| >= |f|
    ///	|0 0 w|   |x| =  |d|
    ///
    ///  СМ таблицу из A,b,c параметров
    /// </summary>
    /// <param name="A"> Ax <= b   -> (A|I)(x|w) = b </param>
    /// <param name="c"> (c,x) ->((-c|0),(x|w)) </param>
    /// <param name="ineq"> знак неравентсва =, >=, <= </param>
    /// <param name="b"></param>
    ///( A|I)  b
    ///(-c|0)  F(x,c)
    public static Pair<ArrayList<Integer>,Matrix> buildSymplexTable(Matrix A, Vector c, Vector b, ArrayList<Sign> ineq)throws Exception
    {
        if (A.rows() != b.size())
        {
            throw new Exception("Incorrect symplex problem");
        }
        if (A.cols() != c.size())
        {
            throw new Exception("Incorrect symplex problem");
        }

        Matrix table = new Matrix(A);

        ArrayList<Integer> basis = new ArrayList<Integer>();

        int cntr = 0;

        Vector row;

        for (int n_row = 0; n_row < A.rows(); n_row++)
        {
            row = table.getRows().get(n_row);

            basis.add(-1);

            for (int j = 0; j < b.size(); j++)
            {
                if (ineq.get(j) == Sign.Equal)
                {
                    basis.set(basis.size() - 1, j);
                    continue;
                }
                if (ineq.get(j) ==Sign.More)
                {
                    if (cntr == j)
                    {
                        basis.set(basis.size() - 1, c.size() + j) ;
                        row.pushBack(-1.0);
                        continue;
                    }
                    row.pushBack(0.0);
                    continue;
                }
                if (ineq.get(j) == Sign.Less)
                {
                    if (cntr == j)
                    {
                        basis.set(basis.size() - 1, c.size() + j);
                        row.pushBack(1.0);
                        continue;
                    }
                    row.pushBack(0.0);
                    continue;
                }
            }
            row.pushBack(b.get(cntr));

            if (b.get(cntr) < 0)
            {
                for (int i = 0; i < row.size(); i++)
                {
                    row.set(i, row.get(i) * -1.0);
                  ///  row[i] *= (-1);
                }
            }
            cntr++;
        }

        Vector C = new Vector(table.cols());

        for (int j = 0; j < C.size(); j++)
        {
            C.set(j, j < c.size() ? -c.get(j) : 0.0);
        }

        table.addRow(C);

        return new Pair<>(basis,table);
    }
    public static Vector symplexSolve(Matrix a, Vector c, Vector b, ArrayList<Sign> ineq, SymplexProblemType mode)throws Exception
    {
        System.out.println("SymplexProblemType : " + String.valueOf(mode)+"\n");

        var system_condition = checkSystem(a, b);

        if (system_condition == SolutionType.None)
        {
            return null;
        }

        if (system_condition == SolutionType.Single)
        {
            return Matrix.linsolve(a, b);
        }

        double a_ik;

        int main_row;

        int main_col;

        Pair<ArrayList<Integer>,Matrix> sympex = buildSymplexTable( a, c, b, ineq);

        Matrix table = sympex.item2;

        ArrayList<Integer> basis = sympex.item1;

        System.out.println("Start symplex table:\n\n" + symplexToString(table, basis));

        while (!isPlanOptimal(table, mode))
        {
            main_col = getMainCol(table, mode);

            if (main_col == -1)
            {
                break;
            }

            main_row = getMainRow(main_col,table);

            if (main_row == -1)
            {
                break;
            }

            basis.set(main_row, main_col);

            a_ik = table.get(main_row,main_col);

            table.rows.get(main_row).mul(1.0 / a_ik);

            for (int i = 0; i < table.rows(); i++)
            {
                if (i == main_row)
                {
                    continue;
                }
                table.rows.get(i).sub(Vector.mul(table.rows.get(main_row),table.get(i,main_col)));
                //     A[i] = A[i] - A[i][main_col] * A[main_row];
            }
            System.out.println("a_main {" + (main_row + 1) + ", " + (main_col + 1) + "} = "+toRationalStr(a_ik)+" \n");
            System.out.println(symplexToString(table, basis));
            System.out.println("current_solution : " + toRationalStr(currentSymplexSolution(table, basis, c.size()))+"\n");
        }
        /// формирование ответа
        return currentSymplexSolution(table, basis, c.size());
    }
    public static Vector symplexSolve(Matrix a, Vector c, Vector b, ArrayList<Sign> ineq)throws Exception
    {
        return  symplexSolve( a,  c,  b,  ineq, SymplexProblemType.Max);
    }
    public static Vector symplexSolve(Matrix a, Vector c, Vector b)throws Exception
    {
        ArrayList<Sign> ineq = new ArrayList<>(b.size());

        for(int i=0;i<b.size();i++)
        {
            ineq.add(Sign.Less);
        }
        return  symplexSolve( a,  c,  b,  ineq, SymplexProblemType.Max);
    }
}

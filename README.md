
# Лабораторные работы по методам оптимизации.
## Определения и обозначения.
1. **Промежуток неопределённости** - для методов нулевого порядка - область в которой мы ожидаем обнаружить экстремум функции.
2. **Целевая функция** - гладкая дифференциремая (до второго порядка производной) функция для которой мы ищем экстремум.
3. **Измерение целевой функции** - определение значения **целевой функции** в точке.
4. **lhs** и **rhs** - левая и правая граница **промежутка неопределённости**.
5. **eps** или **accuracy** или $$\varepsilon$$ - точность с которой мы ищем экстемум функции. 
6. **Унимодальная функция** - функция с одним **глобальным** экстремумом на всей области определения.
7. **Мультимодальная функция** - функция с множеством **локальных** экстремумом на всей области определения.
8. Исследование функции **целевой функции** **f** на монотонность в окружности точки **x** радиусом **eps** - определение направления возрастания или убывания функции путём сравнения значений **f(x + eps)** и **f(x - eps)**.
9. Констатнты золотого сечения:  
   **$$\psi = 0.61803398874989484820$$**  
   **$$\phi = 1.61803398874989484820$$**.
## Требования для оформления кода реализации заданий лабораторных  работ.
1. **C++**
   - Лаборатоные работы №1-3 написать в процедурном стиле;
   - Передавать целевую функцию в виде указателя:  
     **```typedef F64(*function_1d)(const F64);```**  
     или для скалярной функции векторного аргумента:  
     **```typedef F64(*function_nd)(const numerics::vector_f64&);```**  
     или использовать:  
     **```std::function<F64(F64)>```**  
     **```std::function<F64(const numerics::vector_f64)>```**;
   - Прототипы функций писать в **".h"** файлах, а реализацию в **".cpp"**;
   - Каждая лабораторная работа реализуется в своей паре  **".h"** и **".cpp"** файлах;
   - Не мешать многомерные методы с одномерными;
   - Функция **"main"** в отдельном файле;
   - Лабораторную работу №4 предпочтительно реализовать используя ООП.
   - Для работы с векторами и матрицами использовать набор классов из **"numerics"** или любую альтернативу на ваш выбор.
3. **Java**
   - Лаборатоные работы №1-3 написать в отдельных классах в виде набора статических методов;
   - Передавать целевую функцию в виде интерфейса и использовать **```@FunctionalInterface```**;
   - Каждая лабораторная работа реализуется в отдельном  **".java"** файле;
   - Не мешать многомерные методы с одномерными;
   - Функция **"main"** в отдельном файле;
   - Лабораторную работу №4 предпочтительно реализовать используя ООП.
   - Для работы с векторами и матрицами использовать набор классов из **"mathUtils"** или любую альтернативу на ваш выбор.
5. **C#**
   - Лаборатоные работы №1-3 написать в отдельных статических классах в виде набора статических методов;
   - Передавать целевую функцию в виде делегатов, например, использовать   
     **```public delegate double Function1D(double x)```**;
   - Каждая лабораторная работа реализуется в отдельном  **".cs"** файле;
   - Не мешать многомерные методы с одномерными;
   - Функция **"Main"** в отдельном файле;
   - Лабораторную работу №4 предпочтительно реализовать используя ООП;
   - Для работы с векторами и матрицами использовать набор классов из **"MathUtils"** или любую альтернативу на ваш выбор.
7. **Python**
   - Каждый метод поиска из лабораторных №1-3 реализовать в отдельных **.py** модулях;
   - Модули лаборатоных работ №1-3 одбъединить в пакеты;
   - Обязательно использовать **аннотации типов Python**;
   - Передавать целевую функцию в виде ссылки, использовать аннотацию  
     **```function: Callable[[float],float]```** или  
     **```function: Callable[[np.ndarray],float]```**;
   - Каждая лабораторная работа реализуется в отдельном пакете, а метод в модуле;
   - Не мешать многомерные методы с одномерными;
   - Точка входа в отдельном **.py** модуле. Точка входа **```if __name__ == "__main__":...```**;
   - Лабораторную работу №4 предпочтительно реализовать используя ООП.
9. **Остальные языки**
   - Лаборатоные работы №1-3 написать в процедурном стиле;
   - Передавать целевую функцию в виде фагумента метода поиска;
   - Не мешать многомерные методы с одномерными;
   - Точка взода в программу в отдельном файле;
   - Лабораторную работу №4 предпочтительно реализовать используя ООП.

## Лабораторная работа №1. Методы одномерного поиска нулевого порядка.
1. **Дихотомия** или **метод половинного деления** - алгоритм поиска экстремума функции нулевого порядка, который предполагает поиск экстремума функции путём разделения **промежутка неопределённости** пополам и **исследования функции на монотонность** в центральной точке. Дихотомия считается самым неэффективным алгоритмом с точки зрения вычислительной сложности, так как **измерение целевой функции** осущесствляется дважды за одну итерацию. Алгоритм поиска можно свести к следующим основным шагам:
   - Переходим в цикл поиска, который ограничен по точности $$\varepsilon$$ и количеству итераций **maxIterations**;
   - Определяем центральную точку **промежутка неопределённости**: $$x_{c}=\frac{lhs+rhs}{2}$$ и рассчитываем функцию в точках  
     $$x_{l}=x_{c}-\varepsilon$$;  
     $$x_{r}=x_{c}+\varepsilon$$.  
   - Если $$f\left(x_{l}\right)>f\left(x_{r}\right)$$ преобразуем **промежуток неопределённости** в:
     $$\left[x_{c}, rhs\right]$$;
   - Если $$f\left(x_{l}\right)<f\left(x_{r}\right)$$ преобразуем **промежуток неопределённости** в:
     $$\left[lhs, x_{c}\right]$$;
   - Продолжаем до тех пор, пока $$|rhs-lhs|<2\varepsilon$$;
   - Возвращаем результат $$\frac{lhs+rhs}{2}$$.
2. **Золотое сечение** - алгоритм поиска экстремума функции нулевого порядка, который использует пропорцию золотого сечения для разбиения **промежутка неопределённости**. Имеет меньшую вычислительную сложность, чем **метод дихотомии** при одинаковых условиях (**lhs**, **rhs** и **eps**). Количество **измерений целевой функции** равно количеству итераций метода + 2. Алгоритм поиска можно свести к следующим основным шагам:
   - Определяем левую и правую точку разбиения отрезка:  
     $$x_{r}=lhs+\psi\left(rhs-lhs\right)$$;  
     $$x_{l}=rhs-\psi\left(rhs-lhs\right)$$.  
   - В точках $$x_{l},x_{r}$$ делаем **измерения целевой функции**:  
     $$f_{l}=f\left(x_{l}\right)$$;  
     $$f_{r}=f\left(x_{r}\right)$$.  
   - Переходим в цикл поиска, который ограничен по точности $$\varepsilon$$ и количеству итераций **maxIterations**;
   - Если $$f\left(x_{l}\right)>f\left(x_{r}\right)$$ преобразуем **промежуток неопределённости** и остальные параметры:
     $$\left[lhs=x_{l},rhs\right]$$;  
     $$x_{l}=x_{r}$$;  
     $$f_{l}=f_{r}$$;  
     $$x_{r}=lhs+\psi\left(rhs-lhs\right)$$;  
     $$f_{r}=f\left(x_{r}\right)$$.  
   - Если $$f\left(x_{l}\right)<f\left(x_{r}\right)$$ преобразуем **промежуток неопределённости** и остальные параметры:
     $$\left[lhs,rhs=x_{r}\right]$$;  
     $$x_{r}=x_{l}$$;  
     $$f_{r}=f_{l}$$;  
     $$x_{l}=rhs-\psi\left(rhs-lhs\right)$$;  
     $$f_{l}=f\left(x_{l}\right)$$. 
   - Продолжаем до тех пор, пока $$|rhs-lhs|<2\varepsilon$$;
   - Возвращаем результат $$\frac{lhs+rhs}{2}$$.
3. Метод поиска **Фибоначчи** - алгоритм поиска экстремума функции нулевого порядка, который использует пропорцию пары чисел фибоначии для разбиения **промежутка неопределённости**. В отличии от двух методов выше позволяет заранее определить количество итераций метода для достижения поставленной точности по формуле $$L_{n}>\frac{lhs+rhs}{\varepsilon}$$, здесь $$n$$ - номер числа фибоначии/количество итераций. **Метод Фибоначчи** работает точнее, чем **золотое сечение** при одинаковом количестве **измерений функций**.чем  Количество **измерений целевой функции** равно количеству итераций метода + 2. Алгоритм поиска можно свести к следующим основным шагам:
   - Определяем пару чисел фибоначии $$L_{n-1}, L_{n}$$ и номер числа $$n$$, где $$L_{n}>\frac{lhs+rhs}{\varepsilon}$$;
   - Определяем левую и правую точку разбиения отрезка:  
     $$x_{r}=lhs+\frac{L_{n-1}}{L_{n}}\left(rhs-lhs\right)$$;  
     $$x_{l}=lhs+\frac{L_{n}-L_{n-1}}{L_{n}}\left(rhs-lhs\right)$$.  
   - В точках $$x_{l},x_{r}$$ делаем **измерения целевой функции**:  
     $$f_{l}=f\left(x_{l}\right)$$;  
     $$f_{r}=f\left(x_{r}\right)$$.  
   - Переходим в цикл поиска, который ограничен по количеству итераций **maxIterations**;
   - Делаем сдвиг пары чисел фибоначчи:  
     $$L_{n-1}, L_{n}\rightarrow L_{n-2}, L_{n-1}$$;  
   - Если $$f\left(x_{l}\right)>f\left(x_{r}\right)$$ преобразуем **промежуток неопределённости** и остальные параметры:
     $$\left[lhs=x_{l}, rhs\right]$$;  
     $$x_{l}=x_{r}$$;  
     $$f_{l}=f_{r}$$;  
     $$x_{r}=lhs+\frac{L_{n-1}}{L_{n}}\left(rhs-lhs\right))$$;  
     $$f_{r}=f\left(x_{r}\right)$$.  
   - Если $$f\left(x_{l}\right)<f\left(x_{r}\right)$$ преобразуем **промежуток неопределённости** и остальные параметры:
     $$\left[lhs, rhs=x_{r}\right]$$;  
     $$x_{r}=x_{l}$$;  
     $$f_{r}=f_{l}$$;  
     $$x_{l}=lhs+\frac{L_{n}-L_{n-1}}{L_{n}}\left(rhs-lhs\right)$$;  
     $$f_{l}=f\left(x_{l}\right)$$.  
   - Продолжаем до тех пор, пока $$|rhs-lhs|<2\varepsilon$$;
   - Возвращаем результат $$\frac{lhs+rhs}{2}$$.

## Лабораторная работа №2. Методы многомерного поиска нулевого порядка.
Необходимо написать модификации медодов нулевого порядка для поиска по направлению из точки пространства $$\vec{lhs}$$ в точку $$\vec{rhs}$$. Один из этих методов использовать при реализации алгоритма по-координатного спуска.
1. Для языков программирования **c++**, **java**, **c#** использовать набры классов из [**"numerics"**](https://github.com/YuryStrelkov/OptimizationMethods/tree/master/C%2B%2B/OptimizationMethods/numerics), [**"mathUtils"**](https://github.com/YuryStrelkov/OptimizationMethods/tree/master/Java/OptimizationMethods/src/main/java/mathUtils),[**"MathUtils"**](https://github.com/YuryStrelkov/OptimizationMethods/tree/master/C%23/OptimizationMethods/MathUtils).
2. **Дихотомия** или **метод половинного деления** - алгоритм необходимо модифицировать для поиска по направлению из точки пространства $$\vec{lhs}$$ в точку $$\vec{rhs}$$. Областью поиска будет отрезок из $$\vec{lhs}$$ в точку $$\vec{rhs}$$. Количество **измерений целевой функции** равно количеству итераций метода * 2. Алгоритм поиска можно свести к следующим основным шагам:
   - Определяем вектор направления по формуле $$\vec{dir}=\frac{\vec{rhs}-\vec{lhs}}{|\vec{rhs}-\vec{lhs}|}\varepsilon$$  
     Пример определения вектора направления **C++**:  
     ```numerics::vector_f64 dir = numerics::vector_f64::direction(lhs, rhs) * eps;```  
     Пример определения вектора направления **Java**:  
     ```DoubleVector dir = DoubleVector.direction(lhs, rhs).mul(eps);```  
     Пример определения вектора направления **C#**:  
     ```Vector dir = DoubleVector.Direction(lhs, rhs) * accuracy;```  
   - Переходим в цикл поиска, который ограничен по точности $$\varepsilon$$ и количеству итераций **maxIterations**;
   - Определяем центральную точку **промежутка неопределённости**: $$\vec{x_{c}}=\frac{\vec{lhs}+\vec{rhs}}{2}$$ и рассчитываем функцию в точках:  
     $$\vec{x_{l}}=\vec{x_{c}}-\vec{dir}$$;  
     $$\vec{x_{r}}=\vec{x_{c}}+\vec{dir}$$.  
   - Если $$f\left(\vec{x_{l}}\right)>f\left(\vec{x_{r}}\right)$$ преобразуем **промежуток неопределённости** в:
     $$\left[\vec{x_{c}}, \vec{rhs}\right]$$;
   - Если $$f\left(\vec{x_{l}}\right)<f\left(\vec{x_{r}}\right)$$ преобразуем **промежуток неопределённости** в:
     $$\left[\vec{lhs}, \vec{x_{c}}\right]$$;
   - Продолжаем до тех пор, пока $$|\vec{rhs}-\vec{lhs}|<2\varepsilon$$;  
     Пример определения модуля вектора **C++**:  
     ```F64 vector_length = numerics::vector_f64::distance(lhs, rhs);```  
     Пример определения модуля вектора **Java**:  
     ```double vectorLength = DoubleVector.distance(lhs, rhs);```  
     Пример определения модуля вектора **C#**:  
     ```double vectorLength = DoubleVector.Distance(lhs, rhs);```  
   - Возвращаем результат $$\frac{\vec{lhs}+\vec{rhs}}{2}$$.
4. **Золотое сечение** - алгоритм необходимо модифицировать для поиска по направлению из точки пространства $$\vec{lhs}$$ в точку $$\vec{rhs}$$. Областью поиска будет отрезок из $$\vec{lhs}$$ в точку $$\vec{rhs}$$. Количество **измерений целевой функции** равно количеству итераций метода + 2. Алгоритм поиска можно свести к следующим основным шагам:
   - Определяем левую и правую точку разбиения отрезка:  
     $$\vec{x_{r}}=\vec{lhs}+\psi\left(\vec{rhs}-\vec{lhs}\right)$$;  
     $$\vec{x_{l}}=\vec{rhs}-\psi\left(\vec{rhs}-\vec{lhs}\right)$$.  
   - В точках $$\vec{x_{l}},\vec{x_{r}}$$ делаем **измерения целевой функции**:  
     $$f_{l}=f\left(\vec{x_{l}}\right)$$;  
     $$f_{r}=f\left(\vec{x_{r}}\right)$$.  
   - Переходим в цикл поиска, который ограничен по точности $$\varepsilon$$ и количеству итераций **maxIterations**;
   - Если $$f\left(\vec{x_{l}}\right)>f\left(\vec{x_{r}}\right)$$ преобразуем **промежуток неопределённости** и остальные параметры:  
     $$\left[\vec{lhs}=\vec{x_{l}},\vec{rhs}\right]$$;  
     $$\vec{x_{l}}=\vec{x_{r}}$$,  
     $$\vec{f_{l}}=\vec{f_{r}}$$;  
     $$\vec{x_{r}}=\vec{lhs}+\psi\left(\vec{rhs}-\vec{lhs}\right)$$;  
     $$f_{r}=f\left(\vec{x_{r}}\right)$$.  
   - Если $$f\left(\vec{x_{l}}\right)<f\left(\vec{x_{r}}\right)$$ преобразуем **промежуток неопределённости** и остальные параметры:  
     $$\left[\vec{lhs},\vec{rhs}=\vec{x_{r}}\right]$$;  
     $$\vec{x_{r}}=\vec{x_{l}}$$;  
     $$\vec{f_{r}}=\vec{f_{l}}$$;  
     $$\vec{x_{l}}=\vec{rhs}-\psi\left(\vec{rhs}-\vec{lhs}\right)$$;  
     $$f_{l}=f\left(\vec{x_{l}}\right)$$.
   - Продолжаем до тех пор, пока $$|\vec{rhs}-\vec{lhs}|<2\varepsilon$$;
   - Возвращаем результат $$\frac{\vec{lhs}+\vec{rhs}}{2}$$.
3. Метод поиска **Фибоначчи** - алгоритм необходимо модифицировать для поиска по направлению из точки пространства $$\vec{lhs}$$ в точку $$\vec{rhs}$$. Областью поиска будет отрезок из $$\vec{lhs}$$ в точку $$\vec{rhs}$$. Количество итераций метода для достижения поставленной точности можно определить по формуле $$L_{n}>\frac{|\vec{rhs} - \vec{lhs}|}{\varepsilon}$$, здесь $$n$$ - номер числа фибоначии/количество итераций. **Метод Фибоначчи** работает точнее, чем **золотое сечение** при одинаковом количестве **измерений функций**.чем  Количество **измерений целевой функции** равно количеству итераций метода + 2. Алгоритм поиска можно свести к следующим основным шагам:
   - Определяем пару чисел фибоначии $$L_{n-1}, L_{n}$$ и номер числа $$n$$, где $$L_{n}>\frac{|\vec{rhs}-\vec{lhs}|}{\varepsilon}$$;
   - Определяем левую и правую точку разбиения отрезка:  
     $$x_{r}=\vec{lhs}+\frac{L_{n-1}}{L_{n}}\left(\vec{rhs} - \vec{lhs}\right)$$;  
     $$x_{l}=\vec{lhs}+\frac{L_{n}-L_{n-1}}{L_{n}}\left(\vec{rhs} - \vec{lhs}\right)$$.  
   - В точках $$\vec{x_{l}},\vec{x_{r}}$$ делаем **измерения целевой функции**:  
     $$f_{l}=f\left(\vec{x_{l}}\right)$$;  
     $$f_{r}=f\left(\vec{x_{r}}\right)$$.  
   - Переходим в цикл поиска, который ограничен по количеству итераций **maxIterations**;
   - Делаем сдвиг пары чисел фибоначчи: $$L_{n-1}, L_{n}\rightarrow L_{n-2}, L_{n-1}$$;
   - Если $$f\left(\vec{x_{l}}\right)>f\left(\vec{x_{r}}\right)$$ преобразуем **промежуток неопределённости** и остальные параметры:  
     $$\left[\vec{lhs}=\vec{x_{l}}, \vec{rhs}\right]$$;  
     $$\vec{x_{l}}=\vec{x_{r}}$$;  
     $$f_{l}=f_{r}$$;  
     $$\vec{x_{r}}=\vec{lhs}+\frac{L_{n-1}}{L_{n}}\left(\vec{rhs} - \vec{lhs}\right)$$;  
     $$f_{r}=f\left(\vec{x_{r}}\right)$$.
   - Если $$f\left(\vec{x_{l}}\right)<f\left(\vec{x_{r}}\right)$$ преобразуем **промежуток неопределённости** и остальные параметры:  
     $$\left[\vec{lhs}, \vec{rhs}=\vec{x_{r}}\right]$$;  
     $$\vec{x_{r}}=\vec{x_{l}}$$;  
     $$f_{r}=f_{l}$$;  
     $$\vec{x_{l}}=\vec{lhs}+\frac{L_{n}-L_{n-1}}{L_{n}}\left(\vec{rhs} - \vec{lhs}\right)$$;  
     $$f_{l}=f\left(\vec{x_{l}}\right)$$.  
   - Продолжаем до тех пор, пока $$|\vec{rhs} - \vec{lhs}|<2\varepsilon$$;
   - Возвращаем результат $$\frac{\vec{lhs}+\vec{rhs}}{2}$$.
6. Метод покоординатного спуска
   Метод покоординатного спуска позволяет обнаружить эстремум **целевой функции** путём последовательного поиска по направлениям орт пространства, которому принадлежит аргумент функции. Алгоритму необходимо указать точку из которой мы начинаем поис $$\vec{x_{0}}$$, количество итераций, точность $$\varepsilon$$ и длину шага $$\lambda$$ поиска вдоль одного орта. Алгоритм поиска можно свести к следующим основным шагам:
   - Исследуем **целевую функцию** на монотонность в окрестности точки $$\vec{x_{0}}$$ водоль орта $$\vec{e_{0}}$$  
     $$\vec{x_{l}}=\vec{x_{0}}-\varepsilon\vec{e_{0}}$$;  
     $$\vec{x_{r}}=\vec{x_{0}}+\varepsilon\vec{e_{0}}$$.  
   - Если $$f\left(vec{x_{l}}\right)>f\left(vec{x_{r}}\right)$$ преобразуем **промежуток неопределённости** в:
     $$\left[x_{0}, x_{0} + \lambda\vec{e_{0}}\right]$$;
   - Если $$f\left(vec{x_{l}}\right)<f\left(vec{x_{r}}\right)$$ преобразуем **промежуток неопределённости** в:
     $$\left[x_{0}, x_{0} - \lambda\vec{e_{0}}\right]$$;
   - Определяем пару точек: $$\vec{x_{0}}$$, $$\vec{x_{0}} + \lambda\vec{e_{0}}$$
 
## Лабораторная работа №3. Методы многомерного поиска высших порядков. Функции штрафа.

	6) Градиентный спуск спуск
	7) метод сопряжённых градиентов
	9) Метод Ньютона-Рафсона.
	10)Функции внешнего и внутреннего штрафа.
 
## Лабораторная работа №4. Задача линейного программирования

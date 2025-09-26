#include "Multidimensional/SearchMethodsNd.h"
#include "Unidimensional/SearchMethods.h"
#include "Math/VectorN.h"
#include "Math/MatrixMN.h"
#include "Math/Constatnts.h"

double test(double x) {
	return (x - 1.0) * (x - 5.0);
}

double testNd(const struct VectorN* x) 
{
	const double* beg = x->beg;
	const double* end = x->beg + x->size;
	double res = 0;							 
	for (; beg != end; ++beg)
		res += (*beg - 3.0) * (*beg - 3.0);
	return res;
}
/// <summary>
/// Л.Р 1
/// </summary>
inline void zeroOrderMethods() 
{
	struct SearchResult* result = NULL;
	bisection(&result, test, 10.0, 0.0, 1e-6, 1000);
	printResult(&result);

	goldenRatio(&result, test, 10.0, 0.0, 1e-6, 1000);
	printResult(&result);

	fibonacci(&result, test, 10.0, 0.0, 1e-6);
	printResult(&result);

	destroyResult(&result);
}
/// <summary>
/// Л.Р 2 / Л.Р 3(без штрафных функций)
/// </summary>
inline void searchMethodsNd()
{
	struct SearchResultNd* result = NULL;
	struct VectorN* lhs   = vNewFrom(2, 0.00, 0.00);
	struct VectorN* rhs   = vNewFrom(2, 10.0, 10.0);
	struct VectorN* point = vNewFrom(2, 3.0, -1.0);
	bisectionNd  (&result, testNd, lhs, rhs, 1e-6, 1000);
	printResultNd(&result);

	goldenRatioNd(&result, testNd, lhs, rhs, 1e-6, 1000);
	printResultNd(&result);
	// 
	fibonacciNd  (&result, testNd, lhs, rhs, 1e-6);
	printResultNd(&result);

	perCoordinateDescend(&result, testNd, point, 1e-6, 1000);
	printResultNd(&result);

	gradientDescend(&result, testNd, point, 1e-6, 1000);
	printResultNd(&result);

	conjGradientDescend(&result, testNd, point, 1e-6, 1000);
	printResultNd(&result);

	newton (&result, testNd, point, 1e-6, 1000);
	printResultNd(&result);

	destroyResult(&result);
	vDesrtoy     (&point);
	vDesrtoy     (&lhs);
	vDesrtoy     (&rhs);
}

void vectorUsageExample()
{
	/// <summary>
	/// Работа с функциями вектора
	/// </summary>
	struct VectorN* delta = vNewFrom(6, 0.0, 1.3333333333, 2.566, 1222.3, 0.111, 1.04); //Инициализаия через список (6-число элементов списка)
	vPrint(delta, rationalPrintf);// Вывод вектора в консоль
	struct VectorN* vector = NULL; vInit(&vector, 5); //Инициализаия через размер
	vAppend(&vector, 0.5); vPrint(vector, NULL); // Вставка в конец списка
	vAppend(&vector, 0.5); vPrint(vector, NULL); // Вставка в конец списка
	vAppendRange(&vector, delta->beg, delta->end); vPrint(vector, rationalPrintf); //Вставка диапазона в конец списка
	vInsert(&vector, 2, 1.0); // Вставка по индексу
	vInsert(&vector, 2, 2.0); // Вставка по индексу
	vInsert(&vector, 2, 3.0); // Вставка по индексу
	vInsert(&vector, 2, 4.0); // Вставка по индексу
	vInsert(&vector, 2, 5.0); // Вставка по индексу
	vInsert(&vector, 2, 6.0); // Вставка по индексу
	vInsert(&vector, 2, 7.0); // Вставка по индексу
	vInsert(&vector, 2, 8.0); // Вставка по индексу
	vInsert(&vector, 2, 9.0); // Вставка по индексу
	vInsert(&vector, 2, 10.5);// Вставка по индексу
	vPrint(vector, rationalPrintf);
	
	struct VectorN* lhs = vNewFrom(6, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
	struct VectorN* rhs = vNewFrom(6, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
	struct VectorN* res = vNewFrom(6);
	/// <summary>
	/// Математические операции с векторами
	/// </summary>
	printf("lhs + rhs : "); vAddVectorVector(&res, lhs, rhs); vPrint(res, NULL);
	printf("lhs - rhs : "); vSubVectorVector(&res, lhs, rhs); vPrint(res, NULL);
	printf("lhs * rhs : "); vMulVectorVector(&res, lhs, rhs); vPrint(res, NULL);
	printf("lhs / rhs : "); vDivVectorVector(&res, lhs, rhs); vPrint(res, NULL);
	printf("lhs + scl : "); vAddVectorDouble(&res, lhs, 1.0); vPrint(res, NULL);
	printf("lhs - scl : "); vSubVectorDouble(&res, lhs, 1.0); vPrint(res, NULL);
	printf("lhs * scl : "); vMulVectorDouble(&res, lhs, 1.0); vPrint(res, NULL);
	printf("lhs / scl : "); vDivVectorDouble(&res, lhs, 1.0); vPrint(res, NULL);
	printf("scl + rhs : "); vAddDoubleVector(&res, 1.0, lhs); vPrint(res, NULL);
	printf("scl - rhs : "); vSubDoubleVector(&res, 1.0, lhs); vPrint(res, NULL);
	printf("scl * rhs : "); vMulDoubleVector(&res, 1.0, lhs); vPrint(res, NULL);
	printf("scl / rhs : "); vDivDoubleVector(&res, 1.0, lhs); vPrint(res, NULL);
	/// <summary>
	/// Очистка памяти
	/// </summary>
	vDesrtoy(&vector);
	vDesrtoy(&delta);
	vDesrtoy(&lhs);
	vDesrtoy(&rhs);
	vDesrtoy(&res);
}

void matrixUsageExample()
{
	struct MatrixMN* lhs = mNewFrom(16, 16,
		0.8147, 0.4218, 0.2769, 0.7094, 0.8909, 0.3517, 0.5688, 0.2290, 0.2599, 0.4018, 0.7803, 0.6491, 0.4868, 0.6225, 0.9049, 0.0855,
		0.9058, 0.9157, 0.0462, 0.7547, 0.9593, 0.8308, 0.4694, 0.9133, 0.8001, 0.0760, 0.3897, 0.7317, 0.4359, 0.5870, 0.9797, 0.2625,
		0.1270, 0.7922, 0.0971, 0.2760, 0.5472, 0.5853, 0.0119, 0.1524, 0.4314, 0.2399, 0.2417, 0.6477, 0.4468, 0.2077, 0.4389, 0.8010,
		0.9134, 0.9595, 0.8235, 0.6797, 0.1386, 0.5497, 0.3371, 0.8258, 0.9106, 0.1233, 0.4039, 0.4509, 0.3063, 0.3012, 0.1111, 0.0292,
		0.6324, 0.6557, 0.6948, 0.6551, 0.1493, 0.9172, 0.1622, 0.5383, 0.1818, 0.1839, 0.0965, 0.5470, 0.5085, 0.4709, 0.2581, 0.9289,
		0.0975, 0.0357, 0.3171, 0.1626, 0.2575, 0.2858, 0.7943, 0.9961, 0.2638, 0.2400, 0.1320, 0.2963, 0.5108, 0.2305, 0.4087, 0.7303,
		0.2785, 0.8491, 0.9502, 0.1190, 0.8407, 0.7572, 0.3112, 0.0782, 0.1455, 0.4173, 0.9421, 0.7447, 0.8176, 0.8443, 0.5949, 0.4886,
		0.5469, 0.9340, 0.0344, 0.4984, 0.2543, 0.7537, 0.5285, 0.4427, 0.1361, 0.0497, 0.9561, 0.1890, 0.7948, 0.1948, 0.2622, 0.5785,
		0.9575, 0.6787, 0.4387, 0.9597, 0.8143, 0.3804, 0.1656, 0.1067, 0.8693, 0.9027, 0.5752, 0.6868, 0.6443, 0.2259, 0.6028, 0.2373,
		0.9649, 0.7577, 0.3816, 0.3404, 0.2435, 0.5678, 0.6020, 0.9619, 0.5797, 0.9448, 0.0598, 0.1835, 0.3786, 0.1707, 0.7112, 0.4588,
		0.1576, 0.7431, 0.7655, 0.5853, 0.9293, 0.0759, 0.2630, 0.0046, 0.5499, 0.4909, 0.2348, 0.3685, 0.8116, 0.2277, 0.2217, 0.9631,
		0.9706, 0.3922, 0.7952, 0.2238, 0.3500, 0.0540, 0.6541, 0.7749, 0.1450, 0.4893, 0.3532, 0.6256, 0.5328, 0.4357, 0.1174, 0.5468,
		0.9572, 0.6555, 0.1869, 0.7513, 0.1966, 0.5308, 0.6892, 0.8173, 0.8530, 0.3377, 0.8212, 0.7802, 0.3507, 0.3111, 0.2967, 0.5211,
		0.4854, 0.1712, 0.4898, 0.2551, 0.2511, 0.7792, 0.7482, 0.8687, 0.6221, 0.9001, 0.0154, 0.0811, 0.9390, 0.9234, 0.3188, 0.2316,
		0.8003, 0.7060, 0.4456, 0.5060, 0.6160, 0.9340, 0.4505, 0.0844, 0.3510, 0.3692, 0.0430, 0.9294, 0.8759, 0.4302, 0.4242, 0.4889,
		0.1419, 0.0318, 0.6463, 0.6991, 0.4733, 0.1299, 0.0838, 0.3998, 0.5132, 0.1112, 0.1690, 0.7757, 0.5502, 0.1848, 0.5079, 0.6241
		);
	struct MatrixMN* rhs = mNewFrom(16, 16,
		0.8147, 0.4218, 0.2769, 0.7094, 0.8909, 0.3517, 0.5688, 0.2290, 0.2599, 0.4018, 0.7803, 0.6491, 0.4868, 0.6225, 0.9049, 0.0855,
		0.9058, 0.9157, 0.0462, 0.7547, 0.9593, 0.8308, 0.4694, 0.9133, 0.8001, 0.0760, 0.3897, 0.7317, 0.4359, 0.5870, 0.9797, 0.2625,
		0.1270, 0.7922, 0.0971, 0.2760, 0.5472, 0.5853, 0.0119, 0.1524, 0.4314, 0.2399, 0.2417, 0.6477, 0.4468, 0.2077, 0.4389, 0.8010,
		0.9134, 0.9595, 0.8235, 0.6797, 0.1386, 0.5497, 0.3371, 0.8258, 0.9106, 0.1233, 0.4039, 0.4509, 0.3063, 0.3012, 0.1111, 0.0292,
		0.6324, 0.6557, 0.6948, 0.6551, 0.1493, 0.9172, 0.1622, 0.5383, 0.1818, 0.1839, 0.0965, 0.5470, 0.5085, 0.4709, 0.2581, 0.9289,
		0.0975, 0.0357, 0.3171, 0.1626, 0.2575, 0.2858, 0.7943, 0.9961, 0.2638, 0.2400, 0.1320, 0.2963, 0.5108, 0.2305, 0.4087, 0.7303,
		0.2785, 0.8491, 0.9502, 0.1190, 0.8407, 0.7572, 0.3112, 0.0782, 0.1455, 0.4173, 0.9421, 0.7447, 0.8176, 0.8443, 0.5949, 0.4886,
		0.5469, 0.9340, 0.0344, 0.4984, 0.2543, 0.7537, 0.5285, 0.4427, 0.1361, 0.0497, 0.9561, 0.1890, 0.7948, 0.1948, 0.2622, 0.5785,
		0.9575, 0.6787, 0.4387, 0.9597, 0.8143, 0.3804, 0.1656, 0.1067, 0.8693, 0.9027, 0.5752, 0.6868, 0.6443, 0.2259, 0.6028, 0.2373,
		0.9649, 0.7577, 0.3816, 0.3404, 0.2435, 0.5678, 0.6020, 0.9619, 0.5797, 0.9448, 0.0598, 0.1835, 0.3786, 0.1707, 0.7112, 0.4588,
		0.1576, 0.7431, 0.7655, 0.5853, 0.9293, 0.0759, 0.2630, 0.0046, 0.5499, 0.4909, 0.2348, 0.3685, 0.8116, 0.2277, 0.2217, 0.9631,
		0.9706, 0.3922, 0.7952, 0.2238, 0.3500, 0.0540, 0.6541, 0.7749, 0.1450, 0.4893, 0.3532, 0.6256, 0.5328, 0.4357, 0.1174, 0.5468,
		0.9572, 0.6555, 0.1869, 0.7513, 0.1966, 0.5308, 0.6892, 0.8173, 0.8530, 0.3377, 0.8212, 0.7802, 0.3507, 0.3111, 0.2967, 0.5211,
		0.4854, 0.1712, 0.4898, 0.2551, 0.2511, 0.7792, 0.7482, 0.8687, 0.6221, 0.9001, 0.0154, 0.0811, 0.9390, 0.9234, 0.3188, 0.2316,
		0.8003, 0.7060, 0.4456, 0.5060, 0.6160, 0.9340, 0.4505, 0.0844, 0.3510, 0.3692, 0.0430, 0.9294, 0.8759, 0.4302, 0.4242, 0.4889,
		0.1419, 0.0318, 0.6463, 0.6991, 0.4733, 0.1299, 0.0838, 0.3998, 0.5132, 0.1112, 0.1690, 0.7757, 0.5502, 0.1848, 0.5079, 0.6241
	);
	/// <summary>
	/// Математические операции с матрицами
	/// </summary>
	struct MatrixMN* res = mNew(4,4);
	printf("lhs + rhs :\n"); mAddMatrixMatrix(&res, lhs, rhs); mPrint(res, rationalPrintf);
	printf("lhs - rhs :\n"); mSubMatrixMatrix(&res, lhs, rhs); mPrint(res, rationalPrintf);
	printf("lhs * rhs :\n"); mMulMatrixMatrix(&res, lhs, rhs); mPrint(res, rationalPrintf);
  printf("lhs / rhs :\n"); mDivMatrixMatrix(&res, lhs, rhs); mPrint(res, rationalPrintf);
	printf("lhs + scl :\n"); mAddMatrixDouble(&res, lhs, 1.0); mPrint(res, rationalPrintf);
	printf("lhs - scl :\n"); mSubMatrixDouble(&res, lhs, 1.0); mPrint(res, rationalPrintf);
	printf("lhs * scl :\n"); mMulMatrixDouble(&res, lhs, 1.0); mPrint(res, rationalPrintf);
	printf("lhs / scl :\n"); mDivMatrixDouble(&res, lhs, 1.0); mPrint(res, rationalPrintf);
	printf("scl + rhs :\n"); mAddDoubleMatrix(&res, 1.0, lhs); mPrint(res, rationalPrintf);
	printf("scl - rhs :\n"); mSubDoubleMatrix(&res, 1.0, lhs); mPrint(res, rationalPrintf);
	printf("scl * rhs :\n"); mMulDoubleMatrix(&res, 1.0, lhs); mPrint(res, rationalPrintf);
	printf("scl / rhs :\n"); mDivDoubleMatrix(&res, 1.0, lhs); mPrint(res, rationalPrintf);
	/// <summary>
	/// Вставка в конец строки/стобца
	/// </summary>
	struct MatrixMN* res1 = NULL; mIdentity(&res1, 4, 4);
	struct VectorN * col  = NULL; vInit(&col, res1->rows);
	mAppendCol(&res1, col); mPrint(res1, rationalPrintf);
	struct VectorN* row = NULL; vInit(&row, res1->cols);
	mAppendRow(&res1, row); mPrint(res1, rationalPrintf);
	/// <summary>
  /// Транспонирование матрицы
  /// </summary>
	struct MatrixMN* trans = NULL;
	mTranspose(&trans, res1); mPrint(trans, rationalPrintf);
	/// <summary>
	/// Очистка памяти
	/// </summary>
	mDesrtoy(&lhs);
	mDesrtoy(&rhs);
	mDesrtoy(&res);
	mDesrtoy(&res1);
	mDesrtoy(&trans);
	vDesrtoy(&col);
	vDesrtoy(&row);
}

int main()
{
	matrixUsageExample();
	vectorUsageExample();
	zeroOrderMethods();
	searchMethodsNd();
	return 0;
}
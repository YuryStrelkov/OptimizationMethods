#include "Constatnts.h"
#include "MatrixMN.h"
#include "VectorN.h"
#include <corecrt_malloc.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#define M_AT(matrix, row, col)((matrix)->data->beg + (matrix)->cols * row + col)
#define MBEG(matrix)((matrix)->data->beg)
#define MEND(matrix)(((matrix)->data->beg + ((matrix)->rows * (matrix)->cols)))

void mMultNaive(struct MatrixMN ** res, const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	if(mInit(res, lhs->rows, rhs->cols))
	{
		const uint64_t rowsA = lhs->rows;
		const uint64_t colsA = lhs->cols;
		const uint64_t rowsB = rhs->rows;
		const uint64_t colsB = rhs->cols;
		for (uint64_t row = 0; row < rowsA; ++row)
			for (uint64_t col = 0; col < colsB; ++col)
				for (uint64_t index = 0; index < colsA; ++index)
					*M_AT(*res, row, col) += (*M_AT(lhs, row, index)) * (*M_AT(rhs, index, col));
	}
}
void mMultBlock(struct MatrixMN ** res, const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	mMultNaive(res, lhs, rhs);
// 	if ((lhs && rhs) && (lhs->cols && rhs->rows))
// 	{
// 		int64_t N = 2000, blocksCount = 50, blockIndex, blockRow, blockCol, blockRow, row, col, index;
// 		int64_t elementsPerBlock = N / blocksCount;
// 		mInit(res, lhs->rows, rhs->cols);
// 		const uint64_t rowsA = lhs->rows;
// 		const uint64_t colsA = lhs->cols;
// 		const uint64_t rowsB = rhs->rows;
// 		const uint64_t colsB = rhs->cols;
// #pragma omp parallel for default(shared) private(blockIndex, blockRow, blockCol, blockRow, row, col, index)
// 		for (blockIndex = 0; blockIndex < blocksCount * blocksCount; ++blockIndex) {
// 			blockRow = blockIndex / blocksCount;
// 			blockCol = blockIndex % blocksCount;
// 			for (blockRow = 0; blockRow < blocksCount; ++blockRow)
// 				for (row = blockRow * elementsPerBlock; row < (blockRow + 1) * elementsPerBlock; ++row)
// 					for (col = blockCol * elementsPerBlock; col < (blockCol + 1) * elementsPerBlock; col++)
// 						for (index = blockRow * elementsPerBlock; index < (blockRow + 1) * elementsPerBlock; ++index)
// 							*M_AT(*res, row, col) += (*M_AT(lhs, row, index)) * (*M_AT(rhs, index, col));
// 		}
// 	}
}
void mMult(struct MatrixMN** res, const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	if ((lhs && rhs) && (lhs->cols && rhs->rows)) 
	{
		if (lhs->cols * lhs->rows > 16384 /*128 * 128 */ || rhs->cols * rhs->rows > 16384)
			mMultBlock(res, lhs, rhs);
		else
			mMultNaive(res, lhs, rhs);
	}
}

uint8_t          mIsEmpy   (const struct MatrixMN*matrix)
{
  if (matrix)
    return vIsEmpy(matrix->data);
  return 0;
}
uint8_t          mEquals   (const struct MatrixMN*lhs, const struct MatrixMN*rhs)
{
  if (lhs && rhs) 
  {
    if (lhs->cols == rhs->cols && lhs->rows == rhs->rows)
    {
      return vEquals(lhs->data, rhs->data);
    }
  }
  return 0;
}
void             mPrint    (const struct MatrixMN*matrix, mPrintf printFunction)
{
	if (matrix == NULL)
		return;
	if (printFunction == NULL)
		printFunction = defaultPrintf;
	printf("[\n");
	for(uint64_t row = 0; row < matrix->rows; ++row)
	{
		const double* beg = matrix->data->beg + matrix->cols * row;
		const double* end = beg               + matrix->cols;
		printf(" [");
		for (; (beg != end) && printFunction(*beg); ++beg)
			if (end - 1 != beg)
				printf(", ");
		if (row < matrix->rows - 1 != row)
			printf(" ],\n");
		else
			printf(" ]\n");
	}
	printf("]\n");
}
struct MatrixMN* mNew      (uint64_t rows, uint64_t cols)
{
  struct MatrixMN* matrix = NULL;
  mInit(&matrix, rows, cols);
  return matrix;
}
uint8_t          mInit     (struct MatrixMN** matrix, uint64_t rows, uint64_t cols)
{
  if (rows * cols == 0)
    return 0;
	if (*matrix != NULL)
	{
		if ((*matrix)->rows == rows && (*matrix)->cols == cols)
		{
			vInit(&((*matrix)->data), rows * cols);
			return 1;
		}
		mDesrtoy(matrix);
	}
	if ((*matrix) = (struct MatrixMN*)malloc(sizeof(struct MatrixMN)))
	{
		(*matrix)->rows = rows;
		(*matrix)->cols = cols;
		(*matrix)->data = vNew(rows * cols);
		return 1;
	}
	return 0;
}
struct MatrixMN* mNewFrom  (uint64_t rows, uint64_t cols, ...)
{
	struct MatrixMN* matrix = mNew(rows, cols);
	va_list args;
	va_start(args, cols);
	for (uint64_t index = 0; index < cols * rows; ++index)
		*((matrix)->data->beg + index) = va_arg(args, double);
	va_end(args);
	return matrix;
}
uint8_t          mInitFrom (struct MatrixMN**matrix, uint64_t rows, uint64_t cols, ...)
{
	if (mInit(matrix, rows, cols))
	{
		va_list args;
		va_start(args, cols);
		for (uint64_t index = 0; index < cols * rows; ++index)
			*((*matrix)->data->beg + index) = va_arg(args, double);
		va_end(args);
		return 1;
	}
	return 0;
}
uint8_t          mCopy     (struct MatrixMN**matrix, const struct MatrixMN* source)
{
	if (source)
	{
		mInit(matrix, source->rows, source->cols);
		double* begDst = MBEG(*matrix);
		double* endDst = MEND(*matrix);
		const double* begSrc = MBEG(source);
		const double* endSrc = MEND(source);
		for (; begDst != endDst && begSrc != endSrc; ++begDst, ++begSrc)
			*begDst = *begSrc;
		return 1;
	}
	return 0;
}
uint8_t  mDesrtoy  (struct MatrixMN**matrix)
{
	if (*matrix) 
	{
		vDesrtoy(&(*matrix)->data);
		free(*matrix);
		*matrix = NULL;
	}
}
void     mAppendRow(struct MatrixMN**matrix, struct VectorN* row)
{
	if (*matrix && row && row->size == (*matrix)->cols)
	{
		vAppendRange(&((*matrix)->data), row->beg, row->beg + row->size);
		(*matrix)->rows++;
	}
}
void     mAppendCol(struct MatrixMN**matrix, struct VectorN* col)
{
	if (*matrix && col && col->size == (*matrix)->rows)
	{
		if (vReserve(&(*matrix)->data, (*matrix)->data->size + col->size))
		{
			uint64_t shift = 0;
			for (uint64_t index = 0; index < col->size; ++index, ++shift)
				vInsert(&(*matrix)->data, (*matrix)->cols * (index + 1) + shift, col->beg[index]);
			(*matrix)->cols++; 
		}
	}
}
uint64_t mIndexOfIf(struct MatrixMN* matrix, uint8_t(*predicat)(double))
{
	if (matrix)
		return vIndexOfIf(matrix->data, predicat);
	return -1;
}
uint64_t mIndexOf  (struct MatrixMN* matrix, double value)
{
	if (matrix)
		return mIndexOf(matrix->data, value);
	return -1;
}
void     mRemoveRow(struct MatrixMN**matrix, uint64_t row)
{						    
}						    
void     mRemoveCol(struct MatrixMN**matrix, uint64_t col){}
double   mGet      (struct MatrixMN* matrix, uint64_t row, uint64_t col)
{
	if (matrix) 
	{
		if (matrix->rows > row && matrix->cols > col)
			return vGet(matrix->data, row * matrix->cols + col);
	}
	return 0.0;
}
double*  mGetRef   (struct MatrixMN* matrix, uint64_t row, uint64_t col)
{
	if (matrix)
	{
		if (matrix->rows > row && matrix->cols > col)
			return vGetRef(matrix->data, row * matrix->cols + col);
	}
	return NULL;
}
void     mSet      (struct MatrixMN* matrix, uint64_t row, uint64_t col, double value)
{
	if (matrix)
	{
		if (matrix->rows > row && matrix->cols > col)
			*vGetRef(matrix->data, row * matrix->cols + col) = value;
	}
}
								 
void     mAddDouble(struct MatrixMN** lhs, double                 rhs) 
{
	if (*lhs) vAddDouble(&(*lhs)->data, rhs);
}
void     mAddMatrix(struct MatrixMN** lhs, const struct MatrixMN* rhs) 
{
	if (*lhs && rhs) vAddVector(&(*lhs)->data, rhs->data);
}
void     mSubDouble(struct MatrixMN** lhs, double                 rhs)
{
	if (*lhs) vSubDouble(&(*lhs)->data, rhs);
}
void     mSubMatrix(struct MatrixMN** lhs, const struct MatrixMN* rhs) 
{
	if (*lhs && rhs) vSubVector(&(*lhs)->data, rhs->data);
}
void     mMulDouble(struct MatrixMN** lhs, double                 rhs) 
{
	if (*lhs) vMulDouble(&(*lhs)->data, rhs);
}
void     mMulMatrix(struct MatrixMN** lhs, const struct MatrixMN* rhs)
{
}
void     mDivDouble(struct MatrixMN** lhs, double                 rhs) 
{
	if (*lhs) vDivDouble(&(*lhs)->data, rhs);
}
void     mDivMatrix(struct MatrixMN** lhs, const struct MatrixMN* rhs) 
{
}

void     mAddMatrixDouble(struct MatrixMN** res, const struct MatrixMN* lhs, double                 rhs)
{
	if (mCopy(res, lhs)) vAddDouble(&(*res)->data, rhs);
}
void     mAddDoubleMatrix(struct MatrixMN** res, double                 lhs, const struct MatrixMN* rhs)
{
	if (mCopy(res, rhs)) vAddDouble(&(*res)->data, lhs);
}
void     mAddMatrixMatrix(struct MatrixMN** res, const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	if (mCopy(res, rhs)) vAddVector(&(*res)->data, (lhs)->data);
}
void     mSubMatrixDouble(struct MatrixMN** res, const struct MatrixMN* lhs, double                 rhs)
{
	if (mCopy(res, lhs)) vSubDouble(&(*res)->data, rhs);
}
void     mSubDoubleMatrix(struct MatrixMN** res, double                 lhs, const struct MatrixMN* rhs)
{
	if (mCopy(res, rhs)) vSubDoubleVector(&(*res)->data, lhs, rhs->data);
}
void     mSubMatrixMatrix(struct MatrixMN** res, const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	if (mCopy(res, lhs)) vSubVector(&(*res)->data, (rhs)->data);
}
void     mMulMatrixDouble(struct MatrixMN** res, const struct MatrixMN* lhs, double                 rhs)
{
	if (mCopy(res, lhs)) vMulDouble(&(*res)->data, rhs);
}
void     mMulDoubleMatrix(struct MatrixMN** res, double                 lhs, const struct MatrixMN* rhs)
{
	if (mCopy(res, rhs)) vMulDouble(&(*res)->data, lhs);
}
void     mMulMatrixMatrix(struct MatrixMN** res, const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	mMult(res, lhs, rhs);
}
void     mDivMatrixDouble(struct MatrixMN** res, const struct MatrixMN* lhs, double                 rhs)
{
	if (mCopy(res, lhs)) vDivDouble(&(*res)->data, rhs);
}
void     mDivDoubleMatrix(struct MatrixMN** res, double                 lhs, const struct MatrixMN* rhs)
{
	 if (mInit(res, rhs->rows, rhs->cols)) 
		 vDivDoubleVector(&(*res)->data, lhs, rhs->data);
}
void     mDivMatrixMatrix(struct MatrixMN** res, const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	struct MatrixMN* inv = NULL;
	mInvert (&inv, rhs);
	mMult   ( res, lhs, inv);
	mDesrtoy(&inv);
}

void     mMulMatrixVector(struct VectorN** res, const struct MatrixMN* lhs, const struct VectorN* rhs)
{
	if (lhs && rhs && (lhs)->cols == (rhs)->size) 
	{
		vInit(res, lhs->rows);
		for (uint64_t row = 0; row < lhs->rows; ++row)
			for (uint64_t col = 0; col < lhs->cols; ++col)
				(*res)->beg[row] += lhs->data->beg[row * lhs->cols + col] * rhs->beg[col];
	}
}
void     mMulVectorMatrix(struct VectorN** res, const struct VectorN* lhs, const struct MatrixMN* rhs)
{
	if (lhs && rhs && (rhs)->rows == (lhs)->size)
	{
		vInit(res, rhs->cols);
		for (uint64_t col = 0; col < rhs->cols; ++col)
			for (uint64_t row = 0; row < rhs->rows; ++row)
				(*res)->beg[col] += lhs->beg[col] * rhs->data->beg[row * rhs->cols + col];
	}
}
				 
uint8_t  mLtMatrixMatrix(const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	return (lhs && rhs) ? vLtVectorVector(lhs->data, rhs->data) : 0;
}
uint8_t  mLtMatrixDouble(const struct MatrixMN* lhs, double                 rhs)
{
	return (lhs) ? vLtVectorDouble(lhs->data, rhs) : 0;
}
uint8_t  mLtDoubleMatrix(double                 lhs, const struct MatrixMN* rhs)
{
	return (rhs) ? vLtDoubleVector(lhs, rhs->data) : 0;
}
				 
uint8_t  mGtMatrixMatrix(const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	return (lhs && rhs) ? vGtVectorVector(lhs->data, rhs->data) : 0;
}
uint8_t  mGtMatrixDouble(const struct MatrixMN* lhs, double                 rhs)
{
	return (lhs) ? vGtVectorDouble(lhs->data, rhs) : 0;
}
uint8_t  mGtDoubleMatrix(double                 lhs, const struct MatrixMN* rhs)
{
	return (rhs) ? vGtDoubleVector(lhs, rhs->data) : 0;
}
				 
uint8_t  mLeMatrixMatrix(const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	return (lhs && rhs) ? vLeVectorVector(lhs->data, rhs->data) : 0;
}
uint8_t  mLeMatrixDouble(const struct MatrixMN* lhs, double                 rhs)
{
	return (lhs) ? vLeVectorDouble(lhs->data, rhs) : 0;
}
uint8_t  mLeDoubleMatrix(double                 lhs, const struct MatrixMN* rhs)
{
	return (rhs) ? vLeDoubleVector(lhs, rhs->data) : 0;
}
				 																																	 
uint8_t  mGeMatrixMatrix(const struct MatrixMN* lhs, const struct MatrixMN* rhs)
{
	return (lhs && rhs) ? vGeVectorVector(lhs->data, rhs->data) : 0;
}
uint8_t  mGeMatrixDouble(const struct MatrixMN* lhs, double                 rhs)
{
	return (lhs) ? vGeVectorDouble(lhs->data, rhs) : 0;
}
uint8_t  mGeDoubleMatrix(double                 lhs, const struct MatrixMN* rhs)
{
	return (rhs) ? vGeDoubleVector(lhs, rhs->data) : 0;
}

inline void    _mRowsSwap(struct MatrixMN*  matrix, uint64_t row1, uint64_t row2)
{
	for (uint64_t index = 0; index < matrix->cols; ++index)
		dSwap(M_AT(matrix, row1, index), M_AT(matrix, row2, index));
}
inline uint8_t _lu       (struct VectorN**  indices, struct MatrixMN** lu, const struct MatrixMN* src)
{
	mCopy(lu, src);
	vInit(indices, src->cols + 1);
	uint64_t i, j, k, imax, resolution = src->cols;
	double maxA, absA;
	for (i = 0; i <= resolution; i++)
		*((*indices)->beg + i) = i;
	//Unit permutation matrix, P[N] initialized with N
	for (i = 0; i < resolution; i++) {
		maxA = 0.0;
		imax = i;

		for (k = i; k < resolution; k++)
			if ((absA = fabs(*M_AT(*lu, k, i))) > maxA) {
				maxA = absA;
				imax = k;
			}
		if (maxA < accuracy/* accuracy<param_t>()*/)
		{
			mDesrtoy(lu);
			vDesrtoy(indices);
			return 0; //failure, matrix is degenerate
		}
		if (imax != i) {
			//pivoting P
			dSwap((*indices)->beg + i, (*indices)->beg + imax);
			_mRowsSwap(*lu, i, imax);
			//counting pivots starting from N (for determinant)
			(*((*indices)->beg + resolution))++;
		}
		for (j = i + 1; j < resolution; j++) {
			*M_AT(*lu, j, i) /= *M_AT(*lu, i, i);
			for (k = i + 1; k < resolution; k++)
				*M_AT(*lu, j, k) -= *M_AT(*lu, j, i) * *M_AT(*lu, i, k);
		}
	}
	return 1;  //decomposition done
}
inline void    _invert   (struct MatrixMN** inv,     const struct VectorN* indices, const struct MatrixMN* lu)
{
	struct VectorN* bVector = NULL;
	if(!vInit(&bVector, lu->rows))
		return;
	if(!mInit(inv, lu->rows, lu->cols))
		return;
	for (uint64_t col = 0; col < lu->cols; ++col)
	{
		bVector->beg[col] = 1.0;
		for (uint64_t i = 0; i < lu->rows; i++)
		{
			*M_AT(*inv, i, col) = bVector->beg[(uint64_t)(indices->beg[i])];
			for (uint64_t k = 0; k < i; k++)
				*M_AT(*inv, i, col) -= *M_AT(lu, i, k) * *M_AT(*inv, k, col);
		}

		for (int64_t i = lu->cols - 1; i >= 0; i--) {
			for (uint64_t k = i + 1; k < lu->cols; k++)
				*M_AT(*inv, i, col) -= *M_AT(lu, i, k) * *M_AT(*inv, k, col);
			*M_AT(*inv, i, col) /= *M_AT(lu, i, i);
		}
		bVector->beg[col] = 0.0;
	}
	vDesrtoy(&bVector);
}

uint8_t  mLU      (struct VectorN**  indices, struct MatrixMN** lu, const struct MatrixMN * src)
{
	if(src && src->cols == src->rows)
		return _lu(indices, lu, src);
	return 0;
}
void     mHessian (struct MatrixMN** hess, const struct VectorN * point, double(*function)(const struct VectorN*) )
{
	if (point)
	{
		mInit(hess, point->size, point->size);
		for (uint64_t row = 0; row < (*hess)->rows; ++row)
			for (uint64_t col = 0; col <= row; ++col)
			{
				*M_AT(*hess, col, row) = vPartial2(point, function, row, col, 1e-6);
				*M_AT(*hess, row, col) = *M_AT(*hess, col, row);
			}
	}
}
uint8_t  mLinsolve(struct VectorN**  res, const struct MatrixMN*matA, const struct VectorN*vecB)
{ 
	if(matA &&
		 matA->cols == matA->rows &&
		 matA->rows == vecB->size)
	{
		struct VectorN* indices = NULL;
		struct MatrixMN* lu = NULL;
		if (mLU(&indices, &lu, matA))
		{
			if (!mInit(res, lu->rows, lu->cols))
			{
				for (uint64_t i = 0; i < lu->rows; i++)
				{
					(*res)->beg[i] = vecB->beg[(uint64_t)(indices->beg[i])];
					for (uint64_t k = 0; k < i; k++)
						(*res)->beg[i] -= *M_AT(lu, i, k) * (*res)->beg[k];
				}
				for (int64_t i = lu->cols - 1; i >= 0; i--) {
					for (uint64_t k = i + 1; k < lu->cols; k++)
						(*res)->beg[i] -= *M_AT(lu, i, k) * (*res)->beg[k];
					(*res)->beg[i] /= *M_AT(lu, i, i);
				}
			}
			mDesrtoy(&lu);
			vDesrtoy(&indices);
		}
	}
	return 0;
}
uint8_t  mInvert  (struct MatrixMN** res, const struct MatrixMN* src)
{
	struct VectorN  * indices = NULL;
	struct MatrixMN * lu = NULL;
	if (mLU(&indices, &lu, src)) 
	{
		_invert(res, indices, lu);
		mDesrtoy(&lu);
		vDesrtoy(&indices);
	}
	return 0;
}
void     mIdentity(struct MatrixMN** res, size_t rows, size_t cols)
{
	if (mInit(res, rows, cols)) 
	{
		for (uint64_t index = 0; index < MIN(rows, cols); ++index)
			*M_AT(*res, index, index) = 1.0;
	}
}
void     mOnes    (struct MatrixMN** res, size_t rows, size_t cols)
{
	if (mInit(res, rows, cols))
	{
		for (uint64_t row = 0; row < rows; ++row)
			for (uint64_t col = 0; col < cols; ++col)
			*M_AT(*res, row, col) = 1.0;
	}
}
void     mZeros   (struct MatrixMN** res, size_t rows, size_t cols)
{
	mInit(res, rows, cols);
}
double   mTrace   (const struct MatrixMN* matrix)
{
	double result = 0;
	if (matrix) 
		for (uint64_t index = 0; index < MIN(matrix->rows, matrix->cols); ++index)
			result += *M_AT(matrix, index, index);
	return result;
}
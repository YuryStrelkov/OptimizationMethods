#pragma once
#include <inttypes.h>

typedef struct VectorN;

typedef struct MatrixMN {
	uint64_t rows;
	uint64_t cols;
	struct VectorN * data;
};
typedef uint64_t(*mPrintf)(double);

uint8_t          mIsEmpy   (const struct MatrixMN*);
uint8_t          mEquals   (const struct MatrixMN*, const struct MatrixMN*);
void             mPrint    (const struct MatrixMN*, mPrintf);
struct MatrixMN* mNew      (uint64_t, uint64_t);
uint8_t          mInit     (struct MatrixMN**, uint64_t, uint64_t);
struct MatrixMN* mNewFrom  (uint64_t, uint64_t, ...);
uint8_t          mInitFrom (struct MatrixMN**, uint64_t, uint64_t, ...);
uint8_t          mCopy     (struct MatrixMN**, const struct MatrixMN*);
uint8_t          mDesrtoy  (struct MatrixMN**);
void             mAppendRow(struct MatrixMN**, struct VectorN*);
void             mAppendCol(struct MatrixMN**, struct VectorN*);
uint64_t         mIndexOfIf(struct MatrixMN*, uint8_t(*predicat)(double));
uint64_t         mIndexOf  (struct MatrixMN*, double);
void             mRemoveRow(struct MatrixMN**, uint64_t);
void             mRemoveCol(struct MatrixMN**, uint64_t);
double           mGet      (struct MatrixMN*, uint64_t, uint64_t);
double*          mGetRef   (struct MatrixMN*, uint64_t, uint64_t);
void             mSet      (struct MatrixMN*, uint64_t, uint64_t, double);
/// <summary>
///  Matrix math functions
/// </summary>
void     mAddDouble(struct MatrixMN**, double);                 // matrix += scalar
void     mAddMatrix(struct MatrixMN**, const struct MatrixMN*); // matrix += matrix
void     mSubDouble(struct MatrixMN**, double);							    // matrix -= scalar
void     mSubMatrix(struct MatrixMN**, const struct MatrixMN*); // matrix -= matrix
void     mMulDouble(struct MatrixMN**, double);							    // matrix *= scalar
void     mMulMatrix(struct MatrixMN**, const struct MatrixMN*); // matrix *= matrix
void     mDivDouble(struct MatrixMN**, double);							    // matrix /= scalar
void     mDivMatrix(struct MatrixMN**, const struct MatrixMN*); // matrix /= matrix

void     mAddMatrixDouble(struct MatrixMN**, const struct MatrixMN*, double);								  // matrix = matrix + scalar
void     mAddDoubleMatrix(struct MatrixMN**, double,                 const struct MatrixMN*); // matrix = scalar + matrix
void     mAddMatrixMatrix(struct MatrixMN**, const struct MatrixMN*, const struct MatrixMN*); // matrix = matrix + matrix

void     mSubMatrixDouble(struct MatrixMN**, const struct MatrixMN*, double);                 // matrix = matrix - scalar
void     mSubDoubleMatrix(struct MatrixMN**, double,                 const struct MatrixMN*); // matrix = scalar - matrix
void     mSubMatrixMatrix(struct MatrixMN**, const struct MatrixMN*, const struct MatrixMN*); // matrix = matrix - matrix

void     mMulMatrixDouble(struct MatrixMN**, const struct MatrixMN*, double);                 // matrix = matrix * scalar
void     mMulDoubleMatrix(struct MatrixMN**, double,                 const struct MatrixMN*); // matrix = scalar * matrix
void     mMulMatrixMatrix(struct MatrixMN**, const struct MatrixMN*, const struct MatrixMN*); // matrix = matrix * matrix

void     mDivMatrixDouble(struct MatrixMN**, const struct MatrixMN*, double);                 // matrix = matrix / scalar
void     mDivDoubleMatrix(struct MatrixMN**, double,                 const struct MatrixMN*); // matrix = scalar / matrix
void     mDivMatrixMatrix(struct MatrixMN**, const struct MatrixMN*, const struct MatrixMN*); // matrix = matrix / matrix

void     mMulMatrixVector(struct VectorN**, const struct MatrixMN*, const struct VectorN*);   // vector = matrix * vector
void     mMulVectorMatrix(struct VectorN**, const struct VectorN*, const struct MatrixMN*);   // vector = vector * matrix 
/// <summary>
///  Matrix logical functions
/// </summary>
uint8_t  mLtMatrixMatrix(const struct MatrixMN*, const struct MatrixMN*); // matrix1 < matrix2
uint8_t  mLtMatrixDouble(const struct MatrixMN*, double                ); // matrix  < scalar
uint8_t  mLtDoubleMatrix(double,                 const struct MatrixMN*); // scalar  < matrix
				 
uint8_t  mGtMatrixMatrix(const struct MatrixMN*, const struct MatrixMN*);  // matrix1 > matrix2
uint8_t  mGtMatrixDouble(const struct MatrixMN*, double                );  // matrix  > scalar
uint8_t  mGtDoubleMatrix(double,                 const struct MatrixMN*);  // scalar  > matrix
				 
uint8_t  mLeMatrixMatrix(const struct MatrixMN*, const struct MatrixMN*);  // matrix1 <= matrix2
uint8_t  mLeMatrixDouble(const struct MatrixMN*, double                );  // matrix  <= scalar
uint8_t  mLeDoubleMatrix(double,                 const struct MatrixMN*);  // scalar  <= matrix
				 
uint8_t  mGeMatrixMatrix(const struct MatrixMN*, const struct MatrixMN*);  // matrix1 >= matrix2
uint8_t  mGeMatrixDouble(const struct MatrixMN*, double                );  // matrix  >= scalar
uint8_t  mGeDoubleMatrix(double,                 const struct MatrixMN*);  // scalar  >= matrix
/// <summary>
///  Matrix functions
/// </summary>
void     mHessian    (struct MatrixMN**, struct VectorN*, double(*)(const struct VectorN*));
uint8_t  mLinsolve   (struct VectorN **, const struct MatrixMN*, const struct VectorN*);
uint8_t  mInvert     (struct MatrixMN**, const struct MatrixMN*);
uint8_t  mLU         (struct VectorN **, struct MatrixMN**, const struct MatrixMN*); // matrix1 >= matrix2
void     mIdentity   (struct MatrixMN**, size_t, size_t);
void     mOnes       (struct MatrixMN**, size_t, size_t);
void     mZeros      (struct MatrixMN**, size_t, size_t);
double   mTrace      (const struct MatrixMN*); 
double   mDeterminant(const struct MatrixMN* matrix);
void     mTranspose(struct MatrixMN** transposed, const struct MatrixMN* matrix);
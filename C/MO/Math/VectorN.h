#ifndef __VECTOR_N__
#define __VECTOR_N__
#include <inttypes.h>

typedef struct VectorN {
	uint64_t size;
	double* beg;
	double* end;
};

typedef double  (*functionNdScalar)(const struct VectorN*);
typedef void    (*functionNdVector)(struct VectorN**, const struct VectorN*);
typedef uint64_t(*vPrintf)(double);

uint8_t         vReserve      (struct VectorN**, uint64_t);
uint8_t         vIsEmpy       (const struct VectorN*);
uint8_t         vEquals       (const struct VectorN*, const struct VectorN*);
void            vPrint        (const struct VectorN*, vPrintf);
struct VectorN* vNew          (uint64_t);
uint8_t         vInit         (struct VectorN**, uint64_t);
struct VectorN* vNewFrom      (uint64_t, ...);
uint8_t         vInitFrom     (struct VectorN**, uint64_t, ...);
uint8_t         vCopy         (struct VectorN**, const struct VectorN*);
uint8_t         vDesrtoy      (struct VectorN**);
uint64_t        vCapacity     (struct VectorN*);
void            vAppend       (struct VectorN**, double);
void            vInsert       (struct VectorN**, uint64_t, double);
uint64_t        vIndexOfIf    (struct VectorN*, uint8_t(*predicat)(double));
uint64_t        vIndexOf      (struct VectorN*, double);
uint64_t        vRemove       (struct VectorN**, double);
uint8_t         vRemoveFirst  (struct VectorN**, double);
void            vRemoveAt     (struct VectorN**, uint64_t);
uint8_t         vRemoveFirstIf(struct VectorN**, uint8_t (*predicat)(double));
uint64_t        vRemoveIf     (struct VectorN**, uint8_t (*predicat)(double));
void            vAppendRange  (struct VectorN**, const double*, const double*);
double          vPop          (struct VectorN*);
double*         vPeek         (struct VectorN*);
double          vGet          (struct VectorN*, uint64_t);
double*         vGetRef       (struct VectorN*, uint64_t);
void            vSet          (struct VectorN*, uint64_t, double);
/// <summary>
///  Vector math functions
/// </summary>
void    vAddDouble      (struct VectorN**, double);                // vector += scalar
void    vAddVector      (struct VectorN**, const struct VectorN*); // vector += vector
void    vSubDouble      (struct VectorN**, double);							   // vector -= scalar
void    vSubVector      (struct VectorN**, const struct VectorN*); // vector -= vector
void    vMulDouble      (struct VectorN**, double);							   // vector *= scalar
void    vMulVector      (struct VectorN**, const struct VectorN*); // vector *= vector
void    vDivDouble      (struct VectorN**, double);							   // vector /= scalar
void    vDivVector      (struct VectorN**, const struct VectorN*); // vector /= vector

void    vAddVectorDouble(struct VectorN**, const struct VectorN*, double);								// vector = vector + scalar
void    vAddDoubleVector(struct VectorN**, double,                const struct VectorN*); // vector = scalar + vector
void    vAddVectorVector(struct VectorN**, const struct VectorN*, const struct VectorN*); // vector = vector + vector
			  
void    vSubVectorDouble(struct VectorN**, const struct VectorN*, double);                // vector = vector - scalar
void    vSubDoubleVector(struct VectorN**, double,                const struct VectorN*); // vector = scalar - vector
void    vSubVectorVector(struct VectorN**, const struct VectorN*, const struct VectorN*); // vector = vector - vector
			  
void    vMulVectorDouble(struct VectorN**, const struct VectorN*, double);                // vector = vector * scalar
void    vMulDoubleVector(struct VectorN**, double,                const struct VectorN*); // vector = scalar * vector
void    vMulVectorVector(struct VectorN**, const struct VectorN*, const struct VectorN*); // vector = vector * vector
			  
void    vDivVectorDouble(struct VectorN**, const struct VectorN*, double);                // vector = vector / scalar
void    vDivDoubleVector(struct VectorN**, double,                const struct VectorN*); // vector = scalar / vector
void    vDivVectorVector(struct VectorN**, const struct VectorN*, const struct VectorN*); // vector = vector / vector

uint8_t vLtVectorVector (const struct VectorN*, const struct VectorN*); // vector1 < vector2
uint8_t vLtVectorDouble (const struct VectorN*,                double); // vector  < scalar
uint8_t vLtDoubleVector (double,                const struct VectorN*); // scalar  < vector
											  
uint8_t vGtVectorVector (const struct VectorN*, const struct VectorN*); // vector1 > vector2
uint8_t vGtVectorDouble (const struct VectorN*,                double); // vector  > scalar
uint8_t vGtDoubleVector (double,                const struct VectorN*); // scalar  > vector
											  
uint8_t vLeVectorVector (const struct VectorN*, const struct VectorN*); // vector1 <= vector2
uint8_t vLeVectorDouble (const struct VectorN*, double               ); // vector  <= scalar
uint8_t vLeDoubleVector (double,                const struct VectorN*); // scalar  <= vector
											  
uint8_t vGeVectorVector (const struct VectorN*, const struct VectorN*); // vector1 >= vector2
uint8_t vGeVectorDouble (const struct VectorN*, double               ); // vector  >= scalar
uint8_t vGeDoubleVector (double,                const struct VectorN*); // scalar  >= vector
											  
void     vNormalize     (struct       VectorN**);
double   vLength        (const struct VectorN* );
void     vNormalized    (struct       VectorN**, const struct VectorN*);
double   vDot           (const struct VectorN* , const struct VectorN*);
double   vDistance      (const struct VectorN*,  const struct VectorN*);
void     vDirection     (struct       VectorN**, const struct VectorN*, const struct VectorN*);
double   vPartial       (struct VectorN* , functionNdScalar, uint64_t index, double step);
double   vPartial2      (struct VectorN* , functionNdScalar, uint64_t index1, uint64_t index2, double step);
void     vGradient      (struct       VectorN**, const struct VectorN*, functionNdScalar, double step);
#endif
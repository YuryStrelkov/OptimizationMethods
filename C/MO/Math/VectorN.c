#include "VectorN.h"
#include "Constatnts.h"
#include <corecrt_malloc.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#define VBEG(vect)((vect)->beg)
#define VEND(vect)((vect)->beg + (vect)->size)
#define V_AT(vect, index)((vect)->beg + index)

#define NULL_SAFETY(vect)\
if (vect == NULL){		   \
   return 0;             \
}

#define NULL_SAFETY_VOID(vect)\
if (vect == NULL){		        \
   return;                    \
}

#define UPSCALE_VECTOR(vect)                       \
if (vCapacity(*vect) == (*vect)->size)             \
{																									 \
	struct VectorN* copy = NULL;										 \
	vInit(&copy, (uint64_t)((*vect)->size * 1.5));	 \
	vCopy(&copy, *vect);														 \
	vDesrtoy(vect);																	 \
	*vect = copy;																		 \
}


inline void upscaleVector(struct VectorN** vect)
{
	if (vCapacity(*vect) == (*vect)->size)
	{
		struct VectorN* copy = NULL;
		vInit(&copy, (uint64_t)((*vect)->size * 1.5));
		vCopy(&copy, *vect);
		vDesrtoy(vect);
		*vect = copy;
	}
}

inline uint8_t _isZero(double value)
{
	return fabs(value) < 1e-9;
}

inline uint8_t _gt(double lhs, double rhs)
{
	return lhs > rhs;
}

inline uint8_t _ge(double lhs, double rhs)
{
	return _gt(lhs, rhs) || _isZero(lhs - rhs);
}

inline uint8_t _lt(double lhs, double rhs)
{
	return lhs < rhs;
}

inline uint8_t _le(double lhs, double rhs)
{
	return _lt(lhs, rhs) || _isZero(lhs - rhs);
}

inline double _mul(double lhs, double rhs)
{
	return lhs * rhs;
}

inline double _difMul(double lhs, double rhs)
{
	return (rhs - lhs) * (rhs - lhs);
}

inline void _swap(double* lhs, double* rhs)
{
	double tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
};

uint8_t vReserve(struct VectorN**vector, uint64_t size)
{
	if (*vector == NULL) 
		return vInit(vector, size);
	if (vCapacity(*vector) >= size)
		return 1;
	double* mem = (double*)malloc(sizeof(double) * size);
	memset(mem, 0, sizeof(double) * size);
	memcpy(mem, (*vector)->beg, MIN((*vector)->size, size) * sizeof(double));
	free((*vector)->beg);
	(*vector)->beg = mem;
	(*vector)->end = (*vector)->beg + size;
	return 1;
}

uint8_t         vIsEmpy                   (const struct VectorN* vect)
{
	return vect ? vect->size != 0 : 0;
}
				        									        
double          vReduceVectorVector       (const struct VectorN* lhs, const struct VectorN* rhs, double(*reduceFunction)(double, double))
{
	if (lhs && rhs)
	{
		double* begLhs = VBEG(lhs);
		double* endLhs = VEND(lhs);
		const double* begRhs = VBEG(rhs);
		const double* endRhs = VEND(rhs);
		double result = 0.0;
		for (; begLhs != endLhs && begRhs != endRhs; ++begLhs, ++begRhs)
			result += reduceFunction(*begLhs, *begRhs);
		return result;
	}
	return 0;
}
				        
uint8_t         vLogicalReduceVectorVector(const struct VectorN* lhs, const struct VectorN* rhs, uint8_t(*logiocalFunction)(double, double))
{
	if (lhs && rhs) 
	{
		uint8_t result = 1;
		double* begLhs = VBEG(lhs);
		double* endLhs = VEND(lhs);
		const double* begRhs = VBEG(rhs);
		const double* endRhs = VEND(rhs);
		for (; begLhs != endLhs && begRhs != endRhs && result; ++begLhs, ++begRhs)
			result = logiocalFunction(*begLhs, *begRhs);
		return result;
	}
	return 0;
}
				        
uint8_t         vLogicalReduceDoubleVector(double lhs, const struct VectorN* rhs, uint8_t(*logiocalFunction)(double, double))
{
	if (rhs) 
	{
		uint8_t result = 1;
		const double* begRhs = VBEG(rhs);
		const double* endRhs = VEND(rhs);
		for (; begRhs != endRhs && result; ++begRhs)
			result = logiocalFunction(lhs, *begRhs);
		return result;
	}
	return 0;
}
				        
uint8_t         vLogicalReduceVectorDouble(const struct VectorN* lhs, double rhs, uint8_t(*logiocalFunction)(double, double))
{
	if (lhs)
	{
		uint8_t result = 1;
		const double* begLhs = VBEG(lhs);
		const double* endLhs = VEND(lhs);
		for (; begLhs != endLhs && result; ++begLhs) 
			result = logiocalFunction(*begLhs, rhs);
		return result;
	}
	return 0;
}
				        
uint8_t         vEquals                   (const struct VectorN* lhs, const struct VectorN* rhs)
{
	if (lhs && rhs)
	{
		double* begLhs = VBEG(lhs);
		double* endLhs = VEND(lhs);
		const double* begRhs = VBEG(rhs);
		const double* endRhs = VEND(rhs);
		uint8_t equals = 1;
		for (; begLhs != endLhs && begRhs != endRhs && equals; ++begLhs, ++begRhs)
			equals = _isZero(*begLhs - *begRhs);
		return equals;
	}
	return 0;
}

struct VectorN* vNew                      (uint64_t size) 
{
	struct VectorN* vector = NULL;
	vInit(&vector, size);
	return vector;
}

void            vPrint                    (const struct VectorN* vect, vPrintf printFunction)
{
	if (vect)
	{
		if (printFunction == NULL)
			printFunction = defaultPrintf;
		const double* beg = VBEG(vect);
		const double* end = VEND(vect);
		printf("[");
		for (; (beg != end) && printFunction(*beg); ++beg)
			if (end - 1 != beg)
				printf(", ");
		printf(" ]\n");
	}
}
				        							            
uint8_t         vInit                     (struct VectorN** vect, uint64_t size)
{
	if (*vect)
	{
		if (vCapacity(*vect) >= size) 
		{
			(*vect)->size = size;
			memset(VBEG(*vect), 0, sizeof(double) * size);
			return 1;
		}
		vDesrtoy(vect);
	}
	if ((*vect) = (struct VectorN*)malloc(sizeof(struct VectorN))) 
	{
		(*vect)->size = size;
		(*vect)->beg  = (double*)malloc(sizeof(double) * size);
		(*vect)->end  = VEND(*vect);
		memset(VBEG(*vect), 0, sizeof(double) * size);
		return 1;
	}
	return 0;
}

struct VectorN* vNewFrom                  (uint64_t size, ...) 
{
	struct VectorN * vect = vNew(size);
	if (vect)
	{
		va_list args;
		va_start(args, size);
		for (uint64_t index = 0; index < size; ++index)
			*vGetRef(vect, index) = va_arg(args, double);
		va_end(args);
	}
	return vect;
}

uint8_t         vInitFrom                 (struct VectorN** vect, uint64_t size, ...)
{
	if (vInit(vect, size)) 
	{
		va_list args;
		va_start(args, size);
		for (uint64_t index = 0; index < size; ++index)
			*vGetRef(vect, index) = va_arg(args, double);
		va_end(args);
		return 1;
	}
	return 0;
}
				        							            
uint8_t         vCopy                     (struct VectorN** vectDst, const struct VectorN* vectSrc)
{
	if (vectSrc)
	{
		vInit(vectDst, (vectSrc)->size);
		double* begDst = VBEG(*vectDst);
		double* endDst = VEND(*vectDst);
		const double* begSrc = VBEG(vectSrc);
		const double* endSrc = VEND(vectSrc);
		for (; begDst != endDst && begSrc != endSrc; ++begDst, ++begSrc)
			*begDst = *begSrc;
		return 1;
	}
	return 0;
}
				        							            
uint8_t         vDesrtoy                  (struct VectorN** vect)
{
	if (*vect)
	{
		if (VBEG(*vect))
			free(VBEG(*vect));
		free(*vect);
		*vect = NULL;
		return 1;
	}
	return 0;
}
				        							            
uint64_t        vCapacity                 (struct VectorN* vect)
{
	return  vect ? vect->end - vect->beg : 0;
}
				        							            
void            vAppend                   (struct VectorN** vect, double value)
{
	if (*vect) 
	{
		upscaleVector(vect);
		*(VEND(*vect)) = value;
		++(*vect)->size;
	}
}
				        							            
uint64_t        vIndexOfIf                (struct VectorN* vect, uint8_t(*predicat)(double))
{
	if (vect)
	{
		const double* beg = VBEG(vect);
		const double* end = VEND(vect);
		for (; beg != end; ++beg)
			if (predicat(*beg))
				break;
		if (beg == end)
			return -1;
		return beg - VBEG(vect);
	}
	return -1;
}
				        							            
uint64_t        vIndexOf                  (struct VectorN* vect, double value)
{
	if (vect)
	{
		const double* beg = VBEG(vect);
		const double* end = VEND(vect);
		for (; beg != end; ++beg)
			if (_isZero(*beg - value))
				break;
		if (beg == end)
			return -1;
		return beg - vect->beg;
	}
	return -1;
}
				        							            
void            vInsert                   (struct VectorN** vect, uint64_t index, double value)
{
	if (*vect)
	{
		UPSCALE_VECTOR(vect);
		if (index >= (*vect)->size)
		{
			vAppend(vect, value);
			return;
		}
		double* insertPos = V_AT(*vect, index);
		double* prev = VEND(*vect);
		while (insertPos != prev) {
			*prev = *(prev - 1);
			--prev;
		}
		*((*vect)->beg + index) = value;
		++(*vect)->size;
	}
}
				        							            
uint64_t        vRemove                   (struct VectorN** vect, double value)
{
	if (*vect)
	{
		uint64_t deletions = 0;
		while (vRemoveFirst(vect, value))
			++deletions;
		return deletions;
	}
	return 0;
}
				        							            
uint8_t         vRemoveFirst              (struct VectorN** vect, double value)
{
	if (*vect)
	{
		uint64_t index = vIndexOf(*vect, value);
		if (index == -1)
			return 0;
		vRemoveAt(vect, index);
		return 1;
	}
	return 0;
}
				        							            
void            vRemoveAt                 (struct VectorN** vect, uint64_t index)
{
	if (*vect) 
	{
		if (index >= (*vect)->size)
			return;
		double* curr = V_AT(*vect, index);
		double* last = VEND(*vect);
		while (curr != last) {
			*curr = *(curr + 1);
			++curr;
		}
		--(*vect)->size;
	}
}
				        							            
uint64_t        vRemoveIf                 (struct VectorN** vect, uint8_t(*predicat)(double))
{
	if (*vect)
	{
		uint64_t deletions = 0;
		while (vRemoveFirstIf(vect, predicat))
			++deletions;
		return deletions;
	}
	return 0;
}
				        							            
uint8_t         vRemoveFirstIf            (struct VectorN** vect, uint8_t(*predicat)(double))
{
	if (*vect)
	{
		uint64_t index = vIndexOfIf(*vect, predicat);
		if (index == -1)
			return 0;
		vRemoveAt(vect, index);
		return 1;
	}
	return 0;
}
				        							            
void            vAppendRange              (struct VectorN** vect, const double* beg, const double* end)
{
	if (*vect)
		if(vReserve(vect, (*vect)->size + (end - beg)))
			for (; beg != end; ++beg)
				vAppend(vect, *beg);
}
				        							            
double          vPop                      (struct VectorN* vect)
{
	if (vect)
	{
		if (vect->size)
		{
			--vect->size;
			return *(VEND(vect));
		}
	}
	return 0.0;
}
				        							            
double*         vPeek                     (struct VectorN* vect)
{
	if (vect)
		if (vect->size)
			return (VEND(vect) - 1);
	return NULL;
}
				        							            
double          vGet                      (struct VectorN* vect, uint64_t index)
{
	if (vect)
		if (index < vect->size)
			return *(V_AT(vect, index));
	return 0.0;
}
				        							            
double*         vGetRef                   (struct VectorN* vect, uint64_t index)
{
	if (vect)
		if (index < vect->size)
			return V_AT(vect, index);
	return NULL;
}
				        							            
void            vSet                      (struct VectorN* vect, uint64_t index, double value)
{
	if (vect)
		if (index < vect->size)
			*(V_AT(vect, index)) = value;
}

void            vMathOperatorVector       (struct VectorN** lhs, const struct VectorN* rhs, uint8_t operatorType)
{
	if (*lhs && rhs) 
	{
		double* begLhs = VBEG(*lhs);
		double* endLhs = VEND(*lhs);
		const double* begRhs = VBEG(rhs);
		const double* endRhs = VEND(rhs);
		switch (operatorType)
		{
		case 0:
			for (; begLhs != endLhs && begRhs != endRhs; ++begLhs, ++begRhs) *begLhs += *begRhs;
			break;
		case 1:
			for (; begLhs != endLhs && begRhs != endRhs; ++begLhs, ++begRhs) *begLhs -= *begRhs;
			break;
		case 2:
			for (; begLhs != endLhs && begRhs != endRhs; ++begLhs, ++begRhs) *begLhs *= *begRhs;
			break;
		case 3:
			for (; begLhs != endLhs && begRhs != endRhs; ++begLhs, ++begRhs) *begLhs /= *begRhs;
			break;
		default:
			break;
		}
	}
}
		            									        
void            vMathOperatorDouble       (struct VectorN** lhs, double rhs, uint8_t operatorType)
{
	if (*lhs)
	{
		double* begLhs = VBEG(*lhs);
		double* endLhs = VEND(*lhs);
		switch (operatorType)
		{
		case 0:
			for (; begLhs != endLhs; ++begLhs) *begLhs += rhs;
			break;
		case 1:
			for (; begLhs != endLhs; ++begLhs) *begLhs -= rhs;
			break;
		case 2:
			for (; begLhs != endLhs; ++begLhs) *begLhs *= rhs;
			break;
		case 3:
			rhs = 1.0 / rhs;
			for (; begLhs != endLhs; ++begLhs) *begLhs *= rhs;
			break;
		default:
			break;
		}
	}
}
		            
void            vAddDouble                (struct VectorN** lhs, double rhs)
{
	vMathOperatorDouble(lhs, rhs, 0);
}
		            					                 
void            vAddVector                (struct VectorN** lhs, const struct VectorN* rhs)
{
	vMathOperatorVector(lhs, rhs, 0);
}
		            					                 
void            vSubDouble                (struct VectorN** lhs, double rhs)
{
	vMathOperatorDouble(lhs, rhs, 1);
}
		            					                 
void            vSubVector                (struct VectorN** lhs, const struct VectorN* rhs)
{
	vMathOperatorVector(lhs, rhs, 1);
}
		            					                 
void            vMulDouble                (struct VectorN** lhs, double rhs)
{
	vMathOperatorDouble(lhs, rhs, 2);
}
		            					                 
void            vMulVector                (struct VectorN** lhs, const struct VectorN* rhs)
{
	vMathOperatorVector(lhs, rhs, 2);
}
		            					                 
void            vDivDouble                (struct VectorN** lhs, double rhs)
{
	vMathOperatorDouble(lhs, rhs, 3);
}
		            					                 
void            vDivVector                (struct VectorN** lhs, const struct VectorN* rhs)
{
	vMathOperatorVector(lhs, rhs, 3);
}
		            								          
void            vAddVectorDouble          (struct VectorN** res, const struct VectorN* lhs, double rhs)
{
	if(vCopy(res, lhs)) vAddDouble (res, rhs);
}
		            								          
void            vAddDoubleVector          (struct VectorN** res, double lhs, const struct VectorN* rhs)
{
	if(vCopy(res, rhs)) vAddDouble (res, lhs);
}
		            								          
void            vAddVectorVector          (struct VectorN** res, const struct VectorN* lhs, const struct VectorN* rhs)
{
	if(vCopy(res, lhs)) vAddVector (res, rhs);
} 
		            								          
void            vSubVectorDouble          (struct VectorN** res, const struct VectorN* lhs, double rhs)
{
	if(vCopy(res, lhs)) vSubDouble (res, rhs);
}
		            								          
void            vSubDoubleVector          (struct VectorN** res, double lhs, const struct VectorN* rhs)
{
	if (vCopy(res, rhs)) 
	{
		double* beg = VBEG(*res);
		double* end = VEND(*res);
		for (; beg != end; ++beg)
			*beg = (lhs - *beg);
	}
}
		            								          
void            vSubVectorVector          (struct VectorN** res, const struct VectorN* lhs, const struct VectorN* rhs)
{
	if(vCopy(res, lhs)) vSubVector (res, rhs);
}
		            								          
void            vMulVectorDouble          (struct VectorN** res, const struct VectorN* lhs, double rhs)
{
	if(vCopy(res, lhs)) vMulDouble (res, rhs);
}
		            								          
void            vMulDoubleVector          (struct VectorN** res, double lhs, const struct VectorN* rhs)
{
	if(vCopy(res, rhs)) vMulDouble (res, lhs);
}
		            								          
void            vMulVectorVector          (struct VectorN** res, const struct VectorN* lhs, const struct VectorN* rhs)
{
	if(vCopy(res, lhs)) vMulVector(res, rhs);
}
		            								          
void            vDivVectorDouble          (struct VectorN** res, const struct VectorN* lhs, double rhs)
{
	if(vCopy(res, lhs)) vDivDouble (res, rhs);
}
		            								          
void            vDivDoubleVector          (struct VectorN** res, double lhs, const struct VectorN* rhs)
{
	if (vCopy(res, rhs)) 
	{
		double* beg = VBEG(*res);
		double* end = VEND(*res);
		for (; beg != end; ++beg)
			*beg = (lhs / *beg);
	}
}
		            								          
void            vDivVectorVector          (struct VectorN** res, const struct VectorN* lhs, const struct VectorN* rhs)
{
	if(vCopy(res, lhs)) vDivVector (res, rhs);
}
				        								          
uint8_t         vLtVectorVector           (const struct VectorN* lhs, const struct VectorN* rhs)
{
	return vLogicalReduceVectorVector(lhs, rhs, _lt);
}
				        								           
uint8_t         vLtVectorDouble           (const struct VectorN* lhs, double rhs)
{
	return vLogicalReduceVectorDouble(lhs, rhs, _lt);
}
				        								           
uint8_t         vLtDoubleVector           (double lhs, const struct VectorN* rhs)
{
	return vLogicalReduceDoubleVector(lhs, rhs, _lt);
}
				        								           
uint8_t         vGtVectorVector           (const struct VectorN* lhs, const struct VectorN* rhs)
{
	return vLogicalReduceVectorVector(lhs, rhs, _gt);
}
				        								           
uint8_t         vGtVectorDouble           (const struct VectorN* lhs, double rhs)
{
}
				        								           
uint8_t         vGtDoubleVector           (double lhs, const struct VectorN* rhs)
{
	return vLogicalReduceDoubleVector(lhs, rhs, _gt);
}
				        								           
uint8_t         vLeVectorVector           (const struct VectorN* lhs, const struct VectorN* rhs)
{
	return vLogicalReduceVectorVector(lhs, rhs, _le);
}
				        								           
uint8_t         vLeVectorDouble           (const struct VectorN* lhs, double rhs)
{
	return vLogicalReduceVectorDouble(lhs, rhs, _le);
}
				        								           
uint8_t         vLeDoubleVector           (double lhs, const struct VectorN* rhs)
{
	return vLogicalReduceDoubleVector(lhs, rhs, _le);
}
				        								           
uint8_t         vGeVectorVector           (const struct VectorN* lhs, const struct VectorN* rhs)
{
	return vLogicalReduceVectorVector(lhs, rhs, _ge);
}
				        								           
uint8_t         vGeVectorDouble           (const struct VectorN* lhs, double rhs)
{
	return vLogicalReduceVectorDouble(lhs, rhs, _ge);
}
				        								           
uint8_t         vGeDoubleVector           (double lhs, const struct VectorN* rhs)
{
	return vLogicalReduceDoubleVector(lhs, rhs, _ge);
}
				        								          
double          vDot                      (const struct VectorN* lhs, const struct VectorN* rhs)
{
	return vReduceVectorVector(lhs, rhs, _mul);
}
				        			                    
double          vLength                   (const struct VectorN*vect) 
{
	return sqrt(vDot(vect, vect));
}
			          				                  
double          vDistance                 (const struct VectorN* lhs, const struct VectorN* rhs)
{
	return sqrt(vReduceVectorVector(lhs, rhs, _difMul));
}
			          					                
void            vDirection                (struct VectorN** direction, const struct VectorN* from, const struct VectorN* to)
{
	if (from && to)
	{
		vSubVectorVector(direction, to, from);
		vMulDouble      (direction, 1.0 / sqrt(vDot(*direction, *direction)));
	}
}
				        					                
void            vNormalize                (struct VectorN** vect)
{
	if(*vect) vMulDouble(vect, 1.0 / vDot(*vect, *vect));
}
		            					                
void            vNormalized               (struct VectorN** vect, const struct VectorN* source)
{
	if (source) 
	{
		vCopy      (vect, source);
		vMulDouble (vect, 1.0 / vDot(vect, vect));
	}
}
				        					                
double          vPartial                  (struct VectorN* point, functionNdScalar function, uint64_t index, double step)
{
	if (point && (point)->size > index)
	{
		*V_AT(point, index) += step;
		double fr = function(point);
		*V_AT(point, index) -= (2.0 * step);
		double fl = function(point);
		*V_AT(point, index) += step;
		return (fr - fl) / step * 0.5; 
	}
	return 0.0;
}

double          vPartial2                 (struct VectorN* point, functionNdScalar function, uint64_t index1, uint64_t index2, double step)
{
	if (point && (point)->size > index2)
	{
		*V_AT(point, index2) += step;
		double fr = vPartial(point, function, index1, step);
		*V_AT(point, index2) -= (2.0 * step);
		double fl = vPartial(point, function, index1, step);
		*V_AT(point, index2) += step;
		return (fr - fl) / step * 0.5;
	}
	return 0.0;
}
			          					                
void            vGradient                 (struct VectorN** gradient, const struct VectorN* point, functionNdScalar function, double step)
{
	if (point)
	{
		vInit(gradient, (point)->size);
		for (uint64_t index = 0; index < (point)->size; ++index)
			*V_AT(*gradient, index) = vPartial(point, function, index, step);
	}
}
				        
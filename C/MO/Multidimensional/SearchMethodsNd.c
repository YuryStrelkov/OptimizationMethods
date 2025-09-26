#include "../Math/Constatnts.h"
#include "../Math/VectorN.h"
#include "../Math/MatrixMN.h"
#include "SearchMethodsNd.h"
#include <corecrt_malloc.h>
#include <math.h>

#define ARGS_VALID_SAFETY(lhs, rhs)\
if (lhs == NULL || rhs == NULL)	   \
return;														 \
if ((lhs)->size != (rhs)->size)	   \
return;

void    setupEmptyResultNd(struct SearchResultNd* result, uint64_t dimension)
{
	(result)->type           = m_noneNd;
	(result)->iterations     = 0;
	(result)->functionProbes = 0;
	(result)->accuracy       = 1e32;
	if ((result)->result == NULL)
		(result)->result = vNew(dimension);
	else
		vInit(&(result)->result, dimension);
};

uint8_t createResultNd    (struct SearchResultNd** result, uint64_t dimension)
{
	if (*result)
	{
		setupEmptyResultNd(*result, dimension);
		return 1;
	}
	if (*result = (struct SearchResultNd*) malloc(sizeof(struct SearchResultNd)))
	{
		memset(*result, 0, sizeof(struct SearchResultNd));
		setupEmptyResultNd(*result, dimension);
		return 1;
	}
	return 0;
}

void    printResultNd     (const struct SearchResultNd** result)
{
	if (*result)
	{
		switch ((*result)->type)
		{
		case m_bisectionNd:
			printf("method:         \"bisection\"\n");
			break;
		case m_goldenRatioNd:
			printf("method:         \"goldenRatio\"\n");
			break;
		case m_fibonacchiNd:
			printf("method:         \"fibonacchi\"\n");
			break;
		case m_perCoordinateDescend:
			printf("method:         \"perCoordinateDescend\"\n");
			break;
		case m_gradientDescend:
			printf("method:         \"gradientDescend\"\n");
			break;
		case m_conjGradientDescend:
			printf("method:         \"conjGradientDescend\"\n");
			break;
		case m_newton:
			printf("method:         \"Newton\"\n");
			break;
		case m_noneNd:
			printf("method:         \"none\"\n");
			return;
		};
		printf("iterations:     %lld\n", (*result)->iterations);
		printf("functionProbes: %lld\n", (*result)->functionProbes);
		printf("accuracy:       %le\n",  (*result)->accuracy);
		printf("result:         ");
		if ((*result)->result)
		{
			vPrint((*result)->result, NULL);
			printf("\n");
		}
		else
			printf("result:         []\n\n");
	}
}

void    destroyResultNd   (struct       SearchResultNd** result)
{
	if (*result)
	{
		if((*result)->result)
			free((*result)->result);
		free(*result);
		*result = NULL;
	}
}

void bisectionNd(struct SearchResultNd** result, functionNd target, struct VectorN* lhs, struct VectorN* rhs, double accuracy, uint64_t maxIters)
{
	ARGS_VALID_SAFETY(lhs, rhs)
	if (!createResultNd(result, (rhs)->size))
		return;
	struct VectorN** _xc  = &(*result)->result;
	struct VectorN*  _dir = NULL;
	struct VectorN*  _lhs = NULL;
	struct VectorN*  _rhs = NULL;
	vCopy(&_lhs, lhs);                
	vCopy(&_rhs, rhs);                
	vDirection(&_dir, lhs, rhs);
	vMulDouble(&_dir, accuracy);
	for (; (*result)->iterations < maxIters && vDistance(_rhs, _lhs) > 2.0 * accuracy; ++((*result)->iterations), (*result)->functionProbes += 2)
	{
		vAddVectorVector        ( _xc, _lhs, _rhs);
		vMulDouble              ( _xc, 0.5);			 
		vSubVector              ( _xc, _dir);
		const double fl = target(*_xc);		         
		vAddVector              ( _xc, _dir);
		vAddVector              ( _xc, _dir);
		const double fr = target(*_xc);		         
		vSubVector              ( _xc, _dir);
		if (fl > fr)
			vCopy(&_lhs, *_xc  );
		else
			vCopy(&_rhs, *_xc  );
	}
	(*result)->type     = m_bisectionNd;
	(*result)->accuracy = vDistance(_rhs, _lhs);
	vDesrtoy(&_dir);
	vDesrtoy(&_lhs);
	vDesrtoy(&_rhs);
};

void goldenRatioNd(struct SearchResultNd** result, functionNd target, const struct VectorN* lhs, const struct VectorN* rhs, double accuracy, uint64_t maxIters)
{
	ARGS_VALID_SAFETY(lhs, rhs)
	if (!createResultNd(result, (rhs)->size))
		return;
	struct VectorN* _delta = NULL;
	struct VectorN* _xl    = NULL;
	struct VectorN* _xr    = NULL;
	struct VectorN* _lhs   = NULL; vCopy(&_lhs, lhs);
	struct VectorN* _rhs   = NULL; vCopy(&_rhs, rhs);
	vSubVectorVector(&_delta, _rhs, _lhs); vMulDouble(&_delta, psi);
	vSubVectorVector(&_xl, _rhs, _delta);
	vAddVectorVector(&_xr, _lhs, _delta);
	double fl = target(_xl);
	double fr = target(_xr);
	for (; (*result)->iterations < maxIters && vDistance(_rhs, _lhs) > 2.0 * accuracy; ++((*result)->iterations))
	{
		if (fl > fr)
		{
			vCopy(&_lhs, _xl);
			vCopy(&_xl,  _xr);
			vSubVectorVector(&_delta, _rhs, _lhs); vMulDouble(&_delta, psi); vAddVectorVector(&_xr, _lhs, _delta);
			fl  = fr;
			fr  = target(_xr);
		}
		else
		{
			vCopy(&_rhs, _xr);
			vCopy(&_xr,  _xl);
			vSubVectorVector(&_delta, _rhs, _lhs); vMulDouble(&_delta, psi); vSubVectorVector(&_xl, _rhs, _delta);
			fr  = fl;
			fl  = target(_xl);
		}
	}
	(*result)->accuracy       = vDistance(_rhs, _lhs);
	(*result)->type           = m_goldenRatioNd;
	(*result)->functionProbes = (*result)->iterations + 2;
	vAddVectorVector(&(*result)->result, _rhs, _lhs); vMulDouble(&(*result)->result, 0.5);
	vDesrtoy(&   _xl);
	vDesrtoy(&   _xr);
	vDesrtoy(&  _lhs);
	vDesrtoy(&  _rhs);
	vDesrtoy(&_delta);
};

void fibonacciNd(struct SearchResultNd** result, functionNd target, const struct VectorN* lhs, const struct VectorN* rhs, double accuracy)
{
	ARGS_VALID_SAFETY(lhs, rhs)
	if (!createResultNd(result, (rhs)->size))
		return;
	double condition = vDistance(rhs, lhs) / accuracy;
	double fib_t = 0.0, fib_1 = 1.0, fib_2 = 1.0;
	int64_t iterations = 0;
	while (fib_2 < condition)
	{
		iterations++;
		fib_t = fib_1;
		fib_1 = fib_2;
		fib_2 += fib_t;
	}
	struct VectorN* _delta = NULL;
	struct VectorN* _xl    = NULL;
	struct VectorN* _xr    = NULL;
	struct VectorN* _lhs   = NULL; vCopy(&_lhs, lhs);
	struct VectorN* _rhs   = NULL; vCopy(&_rhs, rhs);
	vSubVectorVector(&_delta, _rhs, _lhs);
	vMulVectorDouble(&_xl, _delta, (fib_2 - fib_1) / fib_2); vAddVector(&_xl, _lhs);
	vMulVectorDouble(&_xr, _delta,         (fib_1 / fib_2)); vAddVector(&_xr, _lhs);
	double fl = target(_xl);
	double fr = target(_xr);

	for (int64_t index = iterations; index; index--, ++((*result)->iterations))
	{
		fib_t = fib_2 - fib_1;
		fib_2 = fib_1;
		fib_1 = fib_t;
		if (fl > fr)
		{
			vCopy(&_lhs, _xl);
			vCopy(&_xl,  _xr);
			vSubVectorVector(&_delta, _rhs, _lhs);
			vMulVectorDouble(&_xr, _delta, (fib_1 / fib_2)); vAddVector(&_xr, _lhs);
			fl = fr;
			fr = target(_xr);
		}
		else
		{
			vCopy(&_rhs, _xr);
			vCopy(&_xr,  _xl);
			vSubVectorVector(&_delta, _rhs, _lhs);
			vMulVectorDouble(&_xl, _delta, (fib_2 - fib_1) / fib_2); vAddVector(&_xl, _lhs);
			fr = fl;
			fl = target(_xl);
		}
	};
	(*result)->accuracy       = vDistance(_rhs, _lhs);
	(*result)->type           = m_fibonacchiNd;
	(*result)->functionProbes = (*result)->iterations + 2;
	vAddVectorVector(&(*result)->result, _rhs, _lhs); vMulDouble(&(*result)->result, 0.5);
	vDesrtoy(&   _xl);
	vDesrtoy(&   _xr);
	vDesrtoy(&  _lhs);
	vDesrtoy(&  _rhs);
	vDesrtoy(&_delta);
};

void perCoordinateDescend(struct SearchResultNd** result, functionNd target, const struct VectorN* start, double accuracy, uint64_t maxIters)
{
	if (start == NULL)
		return;
	if (!createResultNd(result, (start)->size))
		return;
	struct VectorN* prev = NULL; vCopy(&prev, start);
	struct VectorN* next = NULL; vCopy(&next, start);
	double step = 1.0, xi, y1, y0;
	uint64_t optCoordN = 0, coordId, iteration = 0, totalProbes = 0;

	for (iteration = 0; iteration < maxIters; ++iteration)
	{
		coordId = iteration % next->size;
		double * coordinate = (vGetRef(next, coordId));
		*(coordinate) -= accuracy;
		y0 = target(next);
		*(coordinate) += 2.0 * accuracy;
		y1 = target(next);
		*(coordinate) -= accuracy;
		*coordinate = y0 > y1 ? *coordinate + step : *coordinate - step;
		xi = *(vGetRef(prev, coordId));
		fibonacciNd(result, target, prev, next, accuracy);
		vCopy(&prev, (*result)->result);
		vCopy(&next, (*result)->result);
		totalProbes += (*result)->functionProbes;
		totalProbes += 2;
		if (fabs(*coordinate - xi) < accuracy)
		{
			optCoordN++;
			if (optCoordN == prev->size)
				break;
			continue;
		}
		optCoordN = 0;
	}
	(*result)->type           = m_perCoordinateDescend;
	(*result)->functionProbes = totalProbes;
	(*result)->iterations     = iteration;
	vDesrtoy(&prev);
	vDesrtoy(&next);
}

void gradientDescend     (struct SearchResultNd** result, functionNd target, const struct VectorN* start, double accuracy, uint64_t maxIters)
{
	if (start == NULL)
		return;
	if (!createResultNd(result, (start)->size))
		return;
	struct VectorN* prev = NULL; vCopy(&prev, start);
	struct VectorN* next = NULL; vCopy(&next, start);
	struct VectorN* grad = NULL;
	uint64_t iteration = 0, totalProbes = 0;
	for (; iteration <= maxIters; ++iteration)
	{
		vGradient       (&grad, prev, target, accuracy);
		vSubVectorVector(&next, prev, grad);
		fibonacciNd     (result, target, prev, next, accuracy);
		totalProbes +=  (*result)->functionProbes;
		if (vDistance(prev, (*result)->result) < accuracy)
			break;
		vCopy(&prev, (*result)->result);
	}
	(*result)->type           = m_gradientDescend;
	(*result)->functionProbes = totalProbes;
	(*result)->iterations     = iteration;
	vDesrtoy(&prev);
	vDesrtoy(&next);
}

void conjGradientDescend (struct SearchResultNd** result, functionNd target, const struct VectorN* start, double accuracy, uint64_t maxIters)
{
	if (start == NULL)
		return;
	if (!createResultNd(result, (start)->size))
		return;
	struct VectorN* prev  = NULL; vCopy(&prev, start);
	struct VectorN* next  = NULL; vCopy(&next, start);
	struct VectorN* gCurr = NULL; vGradient(&gCurr, start, target, accuracy);
	struct VectorN* gNext = NULL;
	uint64_t iteration = 0, totalProbes = 0;
	double omega;
	for (; iteration <= maxIters; ++iteration)
	{
		vSubVectorVector(&next, prev, gCurr);
		fibonacciNd     (result, target, prev, next, accuracy);
		totalProbes += (*result)->functionProbes;
		if (vDistance(prev, (*result)->result) < accuracy)
			break;
		vGradient(&gNext, (*result)->result, target, accuracy);
		omega = pow(vLength(gNext), 2.0) / pow(vLength(gCurr), 2.0);
		vMulDouble(&gCurr, omega);
		vSubVector(&gCurr, gNext);
		vCopy     (&prev, (*result)->result);
	}
	(*result)->type           = m_conjGradientDescend;
	(*result)->functionProbes = totalProbes;
	(*result)->iterations     = iteration;
	vDesrtoy(& prev);
	vDesrtoy(& next);
	vDesrtoy(&gCurr);
	vDesrtoy(&gNext);
}

void newton(struct SearchResultNd** result, functionNd target, const struct VectorN* start, double accuracy, uint64_t maxIters)
{
	if (start == NULL)
		return;
	if (!createResultNd(result, (start)->size))
		return;
	struct VectorN  *prev = NULL; vCopy(&prev, start);
	struct VectorN  *next = NULL; vCopy(&next, start);
	struct VectorN  *grad = NULL, *hessXgrad = NULL;
	struct MatrixMN *hess = NULL;
	uint64_t iteration = 0, totalProbes = 0;
	for (; iteration <= maxIters; ++iteration)
	{
		vGradient(&grad, prev, target, accuracy);
		mHessian (&hess, prev, target);
		mMulMatrixVector(&hessXgrad, hess, grad);
		vSubVectorVector(&next, prev, hessXgrad);
		fibonacciNd(result, target, prev, next, accuracy);
		totalProbes += (*result)->functionProbes;
		if (vDistance(prev, (*result)->result) < accuracy)
			break;
		vCopy(&prev, (*result)->result);
	}
	(*result)->type = m_newton;
	(*result)->functionProbes = totalProbes;
	(*result)->iterations = iteration;
	vDesrtoy(&prev);
	vDesrtoy(&next);
	vDesrtoy(&hessXgrad);
	mDesrtoy(&hess);
}

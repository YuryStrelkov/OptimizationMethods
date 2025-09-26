#ifndef __SEARH_METHODS_ND__
#define __SEARH_METHODS_ND__
#include <inttypes.h>

typedef struct VectorN;
typedef double (*functionNd)(const struct VectorN*);

typedef enum SearchMethodNdType {
	m_bisectionNd,
	m_goldenRatioNd,
	m_fibonacchiNd,
	m_perCoordinateDescend,
	m_gradientDescend,
	m_conjGradientDescend,
	m_newton,
	m_noneNd
};

typedef struct SearchResultNd {
	enum SearchMethodNdType type;
	uint64_t                iterations;
	uint64_t                functionProbes;
	double                  accuracy;
	struct VectorN*         result;
};

uint8_t createResultNd (struct       SearchResultNd** result, uint64_t);
void    printResultNd  (const struct SearchResultNd** result);
void    destroyResultNd(struct       SearchResultNd** result);

void bisectionNd         (struct SearchResultNd** result, functionNd target, const struct VectorN* lhs, const struct VectorN* rhs, double accuracy, uint64_t maxIters);
void goldenRatioNd       (struct SearchResultNd** result, functionNd target, const struct VectorN* lhs, const struct VectorN* rhs, double accuracy, uint64_t maxIters);
void fibonacciNd         (struct SearchResultNd** result, functionNd target, const struct VectorN* lhs, const struct VectorN* rhs, double accuracy);
void perCoordinateDescend(struct SearchResultNd** result, functionNd target, const struct VectorN* start, double accuracy, uint64_t maxIters);
void gradientDescend     (struct SearchResultNd** result, functionNd target, const struct VectorN* start, double accuracy, uint64_t maxIters);
void conjGradientDescend (struct SearchResultNd** result, functionNd target, const struct VectorN* start, double accuracy, uint64_t maxIters);
void newton              (struct SearchResultNd** result, functionNd target, const struct VectorN* start, double accuracy, uint64_t maxIters);
#endif 
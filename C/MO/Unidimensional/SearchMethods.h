#ifndef __SEARH_METHODS_1D__
#define __SEARH_METHODS_1D__
#include <inttypes.h>
typedef double (*function1d)(double);

typedef enum SearchMethodType {
	m_bisection,
	m_goldenRatio,
	m_fibonacchi,
	m_none
};

typedef struct SearchResult {
	enum SearchMethodType type;
	uint64_t              iterations;
	uint64_t              functionProbes;
	double                accuracy;
	double                result;
};

uint8_t createResult (struct       SearchResult** result);
void    printResult  (const struct SearchResult** result);
void    destroyResult(struct       SearchResult** result);

void bisection  (struct SearchResult** result, function1d target, double lhs, double rhs, double accuracy, uint64_t maxIters);
void goldenRatio(struct SearchResult** result, function1d target, double lhs, double rhs, double accuracy, uint64_t maxIters);
void fibonacci  (struct SearchResult** result, function1d target, double lhs, double rhs, double accuracy);
#endif
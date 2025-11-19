#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#include <inttypes.h>
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
const double  phi;
const double  psi;
const double  accuracy;
void dSwap(double* lhs, double* rhs);
void iSwap(uint64_t* lhs, uint64_t* rhs);
int defaultPrintf(double val);
int rationalPrintf(double val);
void decimalToRational(double, long[3]);

uint8_t isZero(double value);
uint8_t gt(double lhs, double rhs);
uint8_t ge(double lhs, double rhs);
uint8_t lt(double lhs, double rhs);
uint8_t le(double lhs, double rhs);
double mul(double lhs, double rhs);
double difMul(double lhs, double rhs);

#endif
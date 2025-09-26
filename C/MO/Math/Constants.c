#include "Constatnts.h"
#include <stdio.h>
#include <math.h>

const double phi = 1.61803398874989484820;
const double psi = 0.61803398874989484820;
const double accuracy = 1e-9;

void dSwap(double* lhs, double* rhs)
{
	double tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
};

void iSwap(uint64_t* lhs, uint64_t* rhs)
{
	uint64_t tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
};

int defaultPrintf(double val)
{
	return printf("%*.4lf", 10, val);
}

int rationalPrintf(double val)
{
  long number[3];
  decimalToRational(val, number);
  // number[0] | 0 | 1 | 0 | 0 | 1 | 0 | 1 | 1 |
  // number[1] | 0 | 0 | 1 | 0 | 1 | 1 | 0 | 1 |
  // number[2] | 0 | 0 | 0 | 1 | 0 | 1 | 1 | 1 |
  //---------------------------------------------
  // MASK       | 0 | 1 | 2 | 4 | 3 | 6 | 5 | 7 |
  const uint8_t _case = (number[0] ? 1 : 0) | (number[1] ? 2 : 0) | (number[2] ? 4 : 0);
  switch (_case)
  {
    case 0: return printf("%*ld", 13, 0);
    case 1: return printf("%*ld", 13, number[0]);
    case 2: return printf("%*s",  13, "nan");
    case 3: return printf("%*ld", 13, number[0]);
    case 4: return printf("%*ld", 13, 0);
    case 5: return printf("%*ld", 13, number[0]);
    case 6: return printf("    %*ld/%*ld", 4, number[1], 4, number[2]);
    case 7: return printf("%*ld %*ld/%*ld", 3, number[0], 4, number[1], 4, number[2]);
  }
  return 0;
}

void decimalToRational(double value, long number[3])
{
  long m00 = 1;
  long m01 = 0;
  long m10 = 0;
  long m11 = 1;
  long ai, t, sign = value >= 0 ? 1 : -1;
  double x = fabs(value);

  while ((m10 * (ai = (long)x) + m11)<= 1000) {
    t = m00 * ai + m01;
    m01 = m00;
    m00 = t;
    t = m10 * ai + m11;
    m11 = m10;
    m10 = t;
    if (x == (double)ai) {
      break;
    }   // AF: division by zero
    x = 1.0 / (x - (double)ai);
    if (x > (double)0x7FFFFFFF) {
      break;
    }  // AF: representation failure
  }

  if ((number[0] = (long)(m00 / m10)) != 0) {
    number[1] = (long)(m00 - number[0] * m10);
    number[0] *= sign;
    number[2] = (long)m10;
  }
  else 
  {
    number[1] = (long)(sign * m00);
    number[2] = (long)m10;
  }
}
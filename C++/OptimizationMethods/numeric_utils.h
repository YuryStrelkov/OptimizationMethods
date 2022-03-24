#pragma once
#define MAX_DENOMINATOR 1000
static void decimal_to_rational(const double value, int& rational_part, int& nominator, int& denominator, const int max_den = MAX_DENOMINATOR)
{
	long m[2][2];
	
	m[0][0] = m[1][1] = 1;
	m[0][1] = m[1][0] = 0;
	
	long ai;
	
	double x, startx;
	
	int sign = value >= 0 ? 1 : -1;
	
	x = startx = abs(value);

	while (m[1][0] * (ai = (long)x) + m[1][1] <= max_den) {
		long t;
		t = m[0][0] * ai + m[0][1];
		m[0][1] = m[0][0];
		m[0][0] = t;
		t = m[1][0] * ai + m[1][1];
		m[1][1] = m[1][0];
		m[1][0] = t;
		if (x == (double)ai)
		{
			break;
		}   // AF: division by zero
		x = 1 / (x - (double)ai);
		if (x > (double)0x7FFFFFFF)
		{
			break;
		}  // AF: representation failure
	}
 
	if ((rational_part = m[0][0] / m[1][0]) != 0)
	{
		nominator    = m[0][0] - rational_part * m[1][0];
		
		rational_part *= sign;

		denominator = m[1][0];
		
		return;
	}
	rational_part = 0;

	nominator = sign * m[0][0];

	denominator = m[1][0];
}

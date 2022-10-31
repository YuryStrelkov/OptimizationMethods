#pragma once
#include <string>
#include "common.h"

/// <summary>
/// Конвертирует десятичную запись числа в рациональную, например, для числа 1.666 получим 1 2/3
/// </summary>
/// <param name="value">исходное число</param>
/// <param name="rational_part">целые части</param>
/// <param name="numerator">числитель</param>
/// <param name="denominator">знаменатель</param>
/// <param name="max_den">максимально допустимый знаменатель</param>
static void decimalToRational (const double value, int& rational_part, int& numerator, int& denominator, const int max_den = MAX_DENOMINATOR)
{
	long m[2][2];
	
	m[0][0] = m[1][1] = 1;
	m[0][1] = m[1][0] = 0;
	
	long ai;
	
	double x, startx;
	
	int sign = value >= 0 ? 1 : -1;
	
	x = abs(value);
	
	long t;

	while (m[1][0] * (ai = (long)x) + m[1][1] <= max_den) {
	
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
		numerator = m[0][0] - rational_part * m[1][0];
		
		rational_part *= sign;

		denominator = m[1][0];
		
		return;
	}
	rational_part = 0;

	numerator = sign * m[0][0];

	denominator = m[1][0];
}

static std::string strRational(const double val, const bool full_rational = true)
{
	int r_part;
	int num;
	int denom;

	decimalToRational(val, r_part, num, denom);
	if (num == 0)
	{
		return std::to_string(r_part);
	}
	if (r_part == 0)
	{
		return std::to_string(num) + "/" + std::to_string(denom);
	}

	if (full_rational)
	{
		return std::to_string((num + abs(r_part) * denom) * (r_part >= 0 ? 1 : -1)) + "/" + std::to_string(denom);
	}
	return std::to_string(denom) + " " + std::to_string(num) + "/" + std::to_string(denom);
}

static std::string strRational(const vec_n& val, const bool full_rational = true)
{
	if (val.size() == 0) 
	{
		return "{ empty vector }";
	}
	std::string str = "{ ";
	for (int i = 0; i < val.size() - 1; i++)
	{
		str += strRational(val[i], full_rational);
		str += ", ";
	}
	str += strRational(val[val.size() - 1], full_rational);

	str += " }";
	return str;
}
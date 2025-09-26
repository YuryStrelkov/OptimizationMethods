#pragma once
#ifndef __RATIONAL_H__
#define __RATIONAL_H__
#include "../common.h"

NUMERICS_NAMESPACE_BEGIN
struct lmat {
	I64 m00, m01,
		m10, m11;
	lmat() : m00(1), m01(0), m10(0), m11(1) {}
};
/// <summary>
/// Конвертирует десятичную запись числа в рациональную, например, для числа 1.666 получим 1 2/3
/// </summary>
/// <param name="value">исходное число</param>
/// <param name="rational_part">целые части</param>
/// <param name="numerator">числитель</param>
/// <param name="denominator">знаменатель</param>
/// <param name="max_den">максимально допустимый знаменатель</param>
static void decimal_to_rational(const F64& value, I32& rational_part, I32& numerator, I32& denominator, const I32 max_den = MAX_DENOMINATOR)
{
	lmat mat;
	I64 ai;
	I64 t;
	F64 x = std::abs(value);
	I32 sign = value >= 0 ? 1 : -1;
	while (mat.m10 * (ai = (I64)x) + mat.m11 <= max_den) {

		t = mat.m00 * ai + mat.m01;
		mat.m01 = mat.m00;
		mat.m00 = t;

		t = mat.m10 * ai + mat.m11;
		mat.m11 = mat.m10;
		mat.m10 = t;

		if (x == (F64)ai) break; // AF: division by zero
		x = 1.0 / (x - (F64)ai);
		if (x > (F64)0x7FFFFFFF) break;// AF: representation failure
	}

	if ((rational_part = mat.m00 / mat.m10) != 0)
	{
		numerator = mat.m00 - rational_part * mat.m10;
		rational_part *= sign;
		denominator = mat.m10;
		return;
	}
	rational_part = 0;
	numerator = sign * mat.m00;
	denominator = mat.m10;
};

static std::string rational_str(const I32 int_part,
	const I32 numerator,
	const I32 denomerator,
	const bool& full_rational = true)
{
	// if (numerator == 0) return std::to_string(int_part);
	// if (denomerator / numerator > 1000) return std::to_string(int_part);
	// if (int_part == 0) return std::string(fstring("%i/%i", numerator, denomerator));//; std::to_string(numerator) + "/" + std::to_string(denomerator);
	// if (full_rational)
	// 	return std::string(fstring("%i/%i", (numerator + std::abs(int_part) * denomerator) * (int_part >= 0 ? 1 : -1), std::to_string(denomerator)));
	// return std::string(fstring("%i %i/%i", int_part, numerator, denomerator));// std::to_string(int_part) + " " + std::to_string(numerator) + "/" + std::to_string(denomerator);

	if (numerator == 0) return std::to_string(int_part);
	if (int_part == 0) return std::to_string(numerator) + "/" + std::to_string(denomerator);
	if (full_rational)
		return std::to_string((numerator + std::abs(int_part) * denomerator) *
			(int_part >= 0 ? 1 : -1)) + "/" + std::to_string(denomerator);
	return std::to_string(int_part) + " " + std::to_string(numerator) + "/" + std::to_string(denomerator);
};

static std::string rational_str(const F64 val, const bool full_rational = true)
{
	I32 int_part;
	I32 numerator;
	I32 denomerator;
	decimal_to_rational(val, int_part, numerator, denomerator);
	return rational_str(int_part, numerator, denomerator, full_rational);
}

struct rational_number {
private:
	I32 m_numerator;
	I32 m_denominator;
	I32 m_int_part;
public:

	bool operator==(const rational_number& number)const
	{
		if (number.numerator() != numerator())return false;
		if (number.denominator() != denominator())return false;
		if (number.int_part() != int_part())return false;
		return true;
	}

	friend std::ostream& operator<<(std::ostream& stream, const rational_number& number);

	I32 numerator() const {
		return m_numerator;
	}

	I32 denominator() const {
		return m_denominator;
	}

	I32 int_part() const {
		return m_int_part;
	}

	F64 double_value()const
	{
		return int_part() >= 0 ? int_part() + (1.0 * numerator()) / denominator() :
			int_part() - (1.0 * numerator()) / denominator();
	}

	rational_number(const I32 numerator, const I32 denominator, const I32 intPart) {
		m_numerator = numerator;
		m_denominator = denominator;
		m_int_part = intPart;
	}

	rational_number(const rational_number& other)
		:rational_number(other.numerator(), other.denominator(), other.int_part()) {
	}

	rational_number(const F64 value) {
		decimal_to_rational(value, m_int_part, m_numerator, m_denominator);
	}
};
NUMERICS_NAMESPACE_END
#endif

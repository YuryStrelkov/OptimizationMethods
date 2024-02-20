#pragma once
#include <string>
#include "common.h"
#include "numeric_vector.h"


static int get_factorial(const int& number) {
	int result = 1;
	for (int i = 1; i <= number; i++) result = result * i;
	return result;
};

static int_vector calculate_factorials()
{
	int_vector factorials(128);
	factorials.fill([&](const int& index) {return get_factorial(index); });
	return factorials;
};

static int_vector fibonacci_numbers(const int& index) {
	if (index < 1) return int_vector({ 0 });
	if (index < 2) return int_vector({ 0, 1 });
	int_vector res = int_vector(index);
	res[0] = 0;
	res[1] = 1;
	for (int i = 2; i < index; i++) res[i] = res[i - 1] + res[i - 2];
	return res;
};

static void closest_fibonacci_fair(const double& value, int& fib_n, int& fib_n_1) {
	fib_n = 0;
	fib_n_1 = 0;
	if (value < 1) return;
	fib_n_1 = 1;
	if (value < 2) return;
	int f_tmp;
	while (fib_n < value) {
		f_tmp = fib_n;
		fib_n = fib_n_1;
		fib_n_1 += f_tmp;
	}
};

/// <summary>
/// Конвертирует десятичную запись числа в рациональную, например, для числа 1.666 получим 1 2/3
/// </summary>
/// <param name="value">исходное число</param>
/// <param name="rational_part">целые части</param>
/// <param name="numerator">числитель</param>
/// <param name="denominator">знаменатель</param>
/// <param name="max_den">максимально допустимый знаменатель</param>
static void decimal_to_rational(const double& value, int& rational_part, int& numerator, int& denominator, const int& max_den = MAX_DENOMINATOR)
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
};

static std::string rational_str(const int& int_part,
	const int& numerator,
	const int& denomerator,
	const bool& full_rational = true)
{
	if (numerator == 0) return std::to_string(int_part);
	if (int_part == 0) return std::to_string(numerator) + "/" + std::to_string(denomerator);
	if (full_rational)
		return std::to_string((numerator + std::abs(int_part) * denomerator) *
			(int_part >= 0 ? 1 : -1)) + "/" + std::to_string(denomerator);
	return std::to_string(int_part) + " " + std::to_string(numerator) + "/" + std::to_string(denomerator);
};

static std::string rational_str(const double& val, const bool& full_rational = true)
{
	int int_part;
	int numerator;
	int denomerator;
	decimal_to_rational(val, int_part, numerator, denomerator);
	return rational_str(int_part, numerator, denomerator, full_rational);
}

static std::string rational_str(const vec_n& val, const bool& full_rational = true)
{
	if (val.size() == 0)
	{
		return "{ \"error\": \"empty vector\" }";
	}
	std::string str = "{ ";
	for (int i = 0; i < val.size() - 1; i++)
	{
		str += rational_str(val[i], full_rational);
		str += ", ";
	}
	str += rational_str(val[val.size() - 1], full_rational);

	str += " }";
	return str;
};

struct rational_number {
private:
	int m_numerator;
	int m_denominator;
	int m_int_part;
public:

	bool operator==(const rational_number& number)const
	{
		if (number.numerator() != numerator())return false;
		if (number.denominator() != denominator())return false;
		if (number.int_part() != int_part())return false;
		return true;
	}
	
	friend std::ostream& operator<<(std::ostream& stream, const rational_number& number);

	int numerator() const {
		return m_numerator;
	}

	int denominator() const {
		return m_denominator;
	}

	int int_part() const {
		return m_int_part;
	}

	double double_value()const 
	{
		return int_part() >= 0 ? int_part() + (1.0 * numerator()) / denominator() :
			int_part() - (1.0 * numerator()) / denominator();
 	}

	rational_number(const int& numerator, const int& denominator, const int& intPart) {
		m_numerator = numerator;
		m_denominator = denominator;
		m_int_part = intPart;
	}

	rational_number(const rational_number& other)
		:rational_number(other.numerator(), other.denominator(), other.int_part()) {
	}

	rational_number(const double& value) {
		decimal_to_rational(value, m_int_part, m_numerator, m_denominator);
	}
};

std::ostream& operator<<(std::ostream& stream, const rational_number& number)
{
	return stream << rational_str(number.int_part(), number.numerator(), number.denominator(), false);
}
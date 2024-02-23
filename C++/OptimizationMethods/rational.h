#pragma once
namespace rational
{
	/// <summary>
	/// Конвертирует десятичную запись числа в рациональную, например, для числа 1.666 получим 1 2/3
	/// </summary>
	/// <param name="value">исходное число</param>
	/// <param name="rational_part">целые части</param>
	/// <param name="numerator">числитель</param>
	/// <param name="denominator">знаменатель</param>
	/// <param name="max_den">максимально допустимый знаменатель</param>
	static void decimal_to_rational(const double& value, int32_t& rational_part, int32_t& numerator, int32_t& denominator, const int32_t max_den = MAX_DENOMINATOR)
	{
		long m[2][2];

		m[0][0] = m[1][1] = 1;
		m[0][1] = m[1][0] = 0;

		long ai;

		double x, startx;

		int32_t sign = value >= 0 ? 1 : -1;

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

	static std::string rational_str(const int32_t int_part,
		const int32_t numerator,
		const int32_t denomerator,
		const bool& full_rational = true)
	{
		if (numerator == 0) return std::to_string(int_part);
		if (int_part == 0) return std::to_string(numerator) + "/" + std::to_string(denomerator);
		if (full_rational)
			return std::to_string((numerator + std::abs(int_part) * denomerator) *
				(int_part >= 0 ? 1 : -1)) + "/" + std::to_string(denomerator);
		return std::to_string(int_part) + " " + std::to_string(numerator) + "/" + std::to_string(denomerator);
	};

	static std::string rational_str(const double val, const bool full_rational = true)
	{
		int32_t int_part;
		int32_t numerator;
		int32_t denomerator;
		decimal_to_rational(val, int_part, numerator, denomerator);
		return rational_str(int_part, numerator, denomerator, full_rational);
	}

	static std::string rational_str(const vec_n& val, const bool full_rational = true)
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
		int32_t m_numerator;
		int32_t m_denominator;
		int32_t m_int_part;
	public:

		bool operator==(const rational_number& number)const
		{
			if (number.numerator() != numerator())return false;
			if (number.denominator() != denominator())return false;
			if (number.int_part() != int_part())return false;
			return true;
		}

		friend std::ostream& operator<<(std::ostream& stream, const rational_number& number);

		int32_t numerator() const {
			return m_numerator;
		}

		int32_t denominator() const {
			return m_denominator;
		}

		int32_t int_part() const {
			return m_int_part;
		}

		double double_value()const
		{
			return int_part() >= 0 ? int_part() + (1.0 * numerator()) / denominator() :
				int_part() - (1.0 * numerator()) / denominator();
		}

		rational_number(const int32_t numerator, const int32_t denominator, const int32_t intPart) {
			m_numerator = numerator;
			m_denominator = denominator;
			m_int_part = intPart;
		}

		rational_number(const rational_number& other)
			:rational_number(other.numerator(), other.denominator(), other.int_part()) {
		}

		rational_number(const double value) {
			decimal_to_rational(value, m_int_part, m_numerator, m_denominator);
		}
	};

	std::ostream& operator<<(std::ostream& stream, const rational_number& number)
	{
		return stream << rational_str(number.int_part(), number.numerator(), number.denominator(), false);
	}
};


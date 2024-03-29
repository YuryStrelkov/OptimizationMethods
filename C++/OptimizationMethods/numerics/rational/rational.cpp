#include "rational.h"
std::ostream& operator<<(std::ostream& stream, const rational_number& number)
{
	return stream << rational_str(number.int_part(), number.numerator(), number.denominator(), false);
}
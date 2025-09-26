#include "rational.h"
NUMERICS_NAMESPACE_BEGIN
std::ostream& operator<<(std::ostream& stream, const rational_number& number)
{
	return stream << rational_str(number.int_part(), number.numerator(), number.denominator(), false);
}
NUMERICS_NAMESPACE_END
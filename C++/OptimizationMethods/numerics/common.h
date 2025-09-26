#pragma once
#include <functional>
#include <string>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <cassert>

#if defined(__APPLE__)
// copy from STL lib/may be unsafe...
template <class _Ty, class _Other = _Ty>
_Ty exchange(_Ty& _Val, _Other&& _New_val) noexcept /* strengthened */ {
	// assign _New_val to _Val, return previous _Val
	_Ty _Old_val = static_cast<_Ty&&>(_Val);
	_Val = static_cast<_Other&&>(_New_val);
	return _Old_val;
}
#elif defined(__unix__)
#if defined(__linux__)
#define EXCHANGE std::__exchange
#endif
#elif defined(_WIN32) || defined(_WIN64)
#define EXCHANGE std::exchange
#else
#error "Unknown operating system"
#endif

#define PHI 1.61803398874989484820
#define PSI 0.61803398874989484820
typedef uint8_t  UI8;
typedef uint16_t UI16;
typedef uint32_t UI32;
typedef uint64_t UI64;

typedef int8_t  I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef double F64;
typedef float  F32;
#define DOUBLE_PRECISION      1
#define N_DIM_ACCURACY        1e-5
#define N_DIM_DERIVATIVE_STEP 1e-6
#define N_DIM_ITERS_MAX       1000
#define ACCURACY              1e-5
#define ITERS_MAX             50
#define MAX_DENOMINATOR       1000
#define NUMBER_CHARS_COUNT    11
#define DECIMAL_NUMBERS_REPRESNTATION 0
#define RATIONAL_NUMBERS_REPRESNTATION (!DECIMAL_NUMBERS_REPRESNTATION)
#define MAX(A, B) (A > B ? A : B)
#define MIN(A, B) (A < B ? A : B)
#define DISPLAY_PROGRES (_DEBUG? 1 : 0)
#define NUMERICS_NAMESPACE_BEGIN namespace numerics { 
#define NUMERICS_NAMESPACE_END   } 
#define STR char*
#define CSTR const char*
#define TRUE  1
#define FALSE 0
static auto sum_f = [](const auto& lhs, const auto& rhs) { return lhs + rhs; };
static auto dif_f = [](const auto& lhs, const auto& rhs) { return lhs - rhs; };
static auto div_f = [](const auto& lhs, const auto& rhs) { return lhs / rhs; };
static auto mul_f = [](const auto& lhs, const auto& rhs) { return lhs * rhs; };

#define FORMATTED_STRING_BUFFER_SIZE 2048
template<typename... Args>
CSTR fstring(CSTR formatter, Args... args) {
	static char FORMATTED_STRING_BUFFER[FORMATTED_STRING_BUFFER_SIZE];
	sprintf_s(FORMATTED_STRING_BUFFER, FORMATTED_STRING_BUFFER_SIZE, formatter, args...);
	return FORMATTED_STRING_BUFFER;
}
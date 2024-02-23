#pragma once
#include "common.h"

struct slice 
{
private:
	int32_t m_end;
	int32_t m_begin;
	int32_t m_step;
	//exclusive index
public:
	int32_t end()const {
		return m_end;
	}
	//inclusive index
	int32_t begin()const {
		return m_begin;
	}

	int32_t step()const {
		return m_step;
	}

	slice(): slice(0,0,1) {
	}

	slice(const int32_t begin, const int32_t end) : slice(begin, end, 1) {
	}

	slice(const int32_t begin, const int32_t end, const int32_t step) {
		m_step = step;
		m_begin = begin;
		m_end = end;
	}

	friend std::ostream& operator<<(std::ostream& stream, const slice& slice);
};

std::ostream& operator<<(std::ostream& stream, const slice& slice)
{
	if (slice.step() != 1)
		stream << slice.begin() << ":" << slice.end() << ":" << slice.step();
	else
		stream << slice.begin() << ":" << slice.end();
	return stream;
};

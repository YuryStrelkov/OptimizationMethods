#pragma once
#include "common.h"

struct slice 
{
private:
	int m_end;
	int m_begin;
	int m_step;
	//exclusive index
public:
	int end()const {
		return m_end;
	}
	//inclusive index
	int begin()const {
		return m_begin;
	}

	int step()const {
		return m_step;
	}

	slice(): slice(0,0,1) {
	}

	slice(const int& begin, const int& end) : slice(begin, end, 1) {
	}

	slice(const int& begin, const int& end, const int& step) {
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

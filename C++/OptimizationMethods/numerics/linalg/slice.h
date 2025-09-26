#pragma once
#ifndef __SLICE_H__
#define __SLICE_H__
#include "../common.h"
NUMERICS_NAMESPACE_BEGIN
struct slice
{
private:
	I32 m_end;
	I32 m_begin;
	I32 m_step;
	//exclusive index
public:
	I32 end()const {
		return m_end;
	}
	//inclusive index
	I32 begin()const {
		return m_begin;
	}

	I32 step()const {
		return m_step;
	}

	slice() : slice(0, 0, 1) {
	}

	slice(const I32 begin, const I32 end) : slice(begin, end, 1) {
	}

	slice(const I32 begin, const I32 end, const I32 step) {
		m_step = step;
		m_begin = begin;
		m_end = end;
	}

	friend std::ostream& operator<<(std::ostream& stream, const slice& slice);
};
NUMERICS_NAMESPACE_END
#endif
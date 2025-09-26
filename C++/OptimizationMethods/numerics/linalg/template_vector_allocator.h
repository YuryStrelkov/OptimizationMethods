#pragma once
#ifndef __TEMPLATE_VECTOR_ALLOCATOR__
#define __TEMPLATE_VECTOR_ALLOCATOR__
#include "..\common.h"

template<typename T> struct template_vector_allocator
{
private:
	UI8 validate(const T* src, const T* dst)const
	{
		return src && dst;
	}
public:
	template_vector_allocator() = default;
	~template_vector_allocator() = default;
	T* zero_mem(T* block, const UI64 amount = 0)const noexcept
	{
		STR mem_block = reinterpret_cast<STR> (block);
		if (amount)
			for (UI32 index = 0; index < amount * sizeof(T); index++) *(mem_block++) = '\0';
		else
			for (UI32 index = 0; index < sizeof(T); index++) *(mem_block++) = '\0';
		return block;
	}
	T* allocate(const size_t amount)const noexcept
	{
		return (T*)malloc(sizeof(T) * amount);
	}
	UI8 deallocate(T** block, const size_t amount)const noexcept
	{
		if(!amount) return FALSE;
		if (!*block) return FALSE;
		T* start = *block;
		T* end   = *block + amount;
		while (start != end)
		{
			start->~T();
			start++;
		}
		free(*block);
		*block = nullptr;
		return TRUE;
	}
	void move_copy(T* src, T* dst, const size_t amount)const noexcept
	{
		if (!validate(src, dst))return;
		for (size_t index = 0; index < amount; index++, src++, dst++) *zero_mem(dst) = std::move(*src);
	}
	void const_copy(const T* src, T* dst, const size_t amount)const noexcept
	{
		if (!validate(src, dst))return;
		for (size_t index = 0; index < amount; index++, src++, dst++) *dst = T(*src);
	}
	void move_copy_reverced(T* src, T* dst, const size_t amount)const noexcept
	{
		if (!validate(src, dst))return;
		for (size_t index = 0; index < amount; index++, src--, dst--) *zero_mem(dst) = std::move(*src);
	}
	void const_copy_reverced(const T* src, T* dst, const size_t amount)const noexcept
	{
		if (!validate(src, dst))return;
		for (size_t index = 0; index < amount; index++, src--, dst--) *dst = T(*src);
	}
};
#endif
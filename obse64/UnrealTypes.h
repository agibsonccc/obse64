#pragma once

#include "obse64_common/Types.h"

// base types/containers

template <typename T>
class TArray
{
public:
	T	* m_data;
	u32	m_len;		// valid 
	u32	m_size;		// size of buffer (elements)

	// todo: allocator
};

// 8
class FName
{
public:
	u32		m_idx;	// 0 - index in to global name table
	u32		m_num;	// 8 - unreal names have a number so you can have multiple instances
};

static_assert(sizeof(FName) == 8);

// 10
class FString
{
public:
	TArray <wchar_t>	m_data;
};

static_assert(sizeof(FString) == 0x10);

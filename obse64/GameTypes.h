#pragma once

#include "obse64_common/Types.h"

// base types/containers

// 10
template <typename T>
class BSSimpleList
{
public:
	struct Node
	{
		T		m_data;
		Node	* m_next;
	};

	Node node;
};

static_assert(sizeof(BSSimpleList<void *>) == 0x10);

// 18
template <typename T>
struct NiTArray
{
	virtual ~NiTArray();

//	void	** _vtbl;			// 00
	T *		m_data;				// 08
	u16		m_capacity;			// 10 - init'd to size of preallocation
	u16		m_firstFreeEntry;	// 12 - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	u16		m_numObjs;			// 14 - init'd to 0
	u16		m_growSize;			// 16 - init'd to size of preallocation
};

static_assert(sizeof(NiTArray<void *>) == 0x18);

// 20
template <typename T>
struct NiTLargeArray
{
	virtual ~NiTLargeArray();

//	void	** _vtbl;			// 00
	T *		m_data;				// 08
	u32		m_capacity;			// 10 - init'd to size of preallocation
	u32		m_firstFreeEntry;	// 14 - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	u32		m_numObjs;			// 18 - init'd to 0
	u32		m_growSize;			// 1C - init'd to size of preallocation
};

static_assert(sizeof(NiTLargeArray<void *>) == 0x20);

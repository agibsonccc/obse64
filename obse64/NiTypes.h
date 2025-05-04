#pragma once

#include "obse64_common/Types.h"

// base types/containers

// 20
// derives from NiTMapBase, we don't bother
template <typename T_key, typename T_data>
class NiTMap
{
public:
	virtual ~NiTMap();

	struct NiTMapItem
	{
		NiTMapItem * next;
		T_key		key;
		T_data		data;
	};

	T_data	Get(T_key key)
	{
		u32	bucket = GetBucket(key);

		for(NiTMapItem * iter = buckets[bucket]; iter; iter = iter->next)
		{
			if(Compare(iter->key, key))
			{
				return iter->data;
			}
		}

		return T_data();
	}

	virtual u32				GetBucket(T_key key);					// return hash % numBuckets;
	virtual bool			Compare(T_key lhs, T_key rhs);			// return lhs == rhs;
	virtual void			FillItem(NiTMapItem * item, T_key key, T_data data);
	// item->key = key; item->data = data;
	virtual void			ResetItem(NiTMapItem * item);
	virtual NiTMapItem *	AllocItem(void);					// return new NiTMapItem;
	virtual void			FreeItem(NiTMapItem * item);			// item->data = 0; delete item;

//	void		** _vtbl;	// 00
	u32			numBuckets;	// 08
	u32			pad0C;		// 0C
	NiTMapItem ** buckets;	// 10
	u32			numEntries;	// 18
	u32			pad1C;		// 1C
};

static_assert(sizeof(NiTMap<u32, u32>) == 0x20);

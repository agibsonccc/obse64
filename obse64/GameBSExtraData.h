#pragma once

#include "obse64_common/Types.h"

enum
{
	kExtraData_AltarActorGUID =		0x5E,
};

// 18
class BSExtraData
{
public:
	virtual ~BSExtraData();

	virtual bool	Compare(BSExtraData * rhs);

//	void		** _vtbl;	// 00
	u8			m_type;		// 08
	u8			pad05[7];	// 09 lol
	BSExtraData	* m_next;	// 08
};

// 20
class BaseExtraList
{
public:
	virtual ~BaseExtraList();

//	void		** _vtbl;			// 00
	BSExtraData	* m_data;			// 08
	u8			m_presence[0x10];	// 10 starts from lsb, larger than original
};

#pragma once

#include "UnrealTypes.h"

// object and RTTI system

class UClass;

// 28
class UObject
{
public:
	virtual ~UObject();

//	void	** _vtbl;		// 00
	u32		m_flags;		// 08
	u32		m_objArrayIdx;	// 0C - index in to global object array
	UClass	* m_type;		// 10
	FName	m_name;			// 18
	UObject	* m_outer;		// 20 - parent for purposes of package ownership
};

static_assert(sizeof(UObject) == 0x28);

// 30
class UField : public UObject
{
public:
	virtual ~UField();

	UField	* m_next;	// 28
};

static_assert(sizeof(UField) == 0x30);

// B0
class UStruct : public UField
{
public:
	virtual ~UStruct();

	void	* unk30;	// 30
	u32		unk38;		// 38
	u32		unk3C;		// 3C

	UStruct	* m_parent;	// 40
	UField	* m_fields;	// 48
	UField	* m_field2;	// 50

	u32		unk58;		// 58
	u32		unk5C;		// 5C

	TArray <u8>	unk60;	// 60
	void	* unk70;	// 70
	void	* unk78;	// 78
	void	* unk80;	// 80
	void	* unk88;	// 88
	TArray <u8>	unk90;	// 90
	void	* unkA0;	// A0
	void	* unkA8;	// A8
};

static_assert(sizeof(UStruct) == 0xB0);

// 
class UClass : public UStruct
{
public:
	virtual ~UClass();
};

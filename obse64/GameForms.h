#pragma once

#include "GameTypes.h"

class TESFile;
class TESObjectCELL;

// 08
class BaseFormComponent
{
public:
	BaseFormComponent();
	~BaseFormComponent();	// non-virtual dtor

	virtual void	Unk_00();
	virtual void	Unk_01();
	virtual void	CopyFromBase(BaseFormComponent * src);
	virtual bool	CompareBase(BaseFormComponent * src);

//	void	** _vtbl;	// 00
};

static_assert(sizeof(BaseFormComponent) == 0x08);

// 30
class TESForm : public BaseFormComponent
{
public:
	TESForm();
	virtual ~TESForm();	// 04

	u8	typeID;		// 08
	u8	pad09[3];	// 09
	u32	flags;		// 0C
	u32	refID;		// 10

	BSSimpleList <TESFile *>	modRefList;	// 18

	void	* unk28;	// 28 new in altar
};

static_assert(sizeof(TESForm) == 0x30);

// 20
class ObjectListHead
{
	u32	numEntries;				// 00
	u32	pad04;					// 04
	TESObject		* start;	// 08
	TESObject		* end;		// 10
	TESObjectCELL	* parent;	// 18
};

static_assert(sizeof(ObjectListHead) == 0x20);

// 48
class TESObject : public TESForm
{
public:
	TESObject();
	virtual ~TESObject();

	ObjectListHead	* objectListHead;	// 30
	TESObject		* objectListPrev;	// 38
	TESObject		* objectListNext;	// 40
};

static_assert(sizeof(TESObject) == 0x48);

// 48
class TESBoundObject : public TESObject
{
public:
	TESBoundObject();
	virtual ~TESBoundObject();
};

static_assert(sizeof(TESBoundObject) == 0x48);

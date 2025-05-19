#pragma once

#include "GameTypes.h"

class TESFile;
class TESObjectCELL;
class TESObject;

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
	virtual void Unk_05();
	virtual void Unk_06();
	virtual void Unk_07();
	virtual void Unk_08();
	virtual void Save();
	virtual void Unk_0A();
	virtual void Unk_0B();
	virtual void Unk_0C();
	virtual void Unk_0D();
	virtual void Unk_0E();
	virtual void Unk_0F();
	virtual void MarkAsModified(u32 mask);
	virtual void ClearModified(u32 mask);
	virtual void Unk_12();
	virtual u32 GetSaveSize(u32 mask);
	virtual void SaveGame(u32 mask);
	virtual void LoadGame(u32 mask);
	virtual void Unk_16();
	virtual void Unk_17();
	virtual void Unk_18();
	virtual void Unk_19();
	virtual void Unk_1A();
	virtual void Unk_1B();
	virtual void Unk_1C();
	virtual void Unk_1D();
	virtual void Unk_1E();
	virtual void Unk_1F();
	virtual void Unk_20();
	virtual void Unk_21();
	virtual void Unk_22();
	virtual void Unk_23();
	virtual void Unk_24();
	virtual void Unk_25();
	virtual void Unk_26();
	virtual void Unk_27();
	virtual void Unk_28();
	virtual void Unk_29();
	virtual void Unk_2A();
	virtual void Unk_2B();
	virtual void Unk_2C();
	virtual void Unk_2D();
	virtual void Unk_2E();
	virtual void Unk_2F();
	virtual void Unk_30();
	virtual void Unk_31();
	virtual void Unk_32();
	virtual void Unk_33();
	virtual void Unk_34();
	virtual void Unk_35();
	virtual void Unk_36();
	virtual void Unk_37();
	virtual void Unk_38();
	virtual void Unk_39();
	virtual void Unk_3A();
	virtual void Unk_3B();

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

// 20
class TESContainer : public BaseFormComponent
{
public:
	struct Entry
	{
		s32 count;
		TESForm * type;
	};

	u8	type;		// 08
	u8	pad09[7];	// 09

	BSSimpleList <Entry *> list;	// 10
};

static_assert(sizeof(TESContainer) == 0x20);

// 20
class TESLeveledList : public BaseFormComponent
{
public:
	enum
	{
		kFlags_CalcAllLevels = 1,
		kFlags_CalcEachInCount = 2,
	};

	// 18
	struct Entry
	{
		u16	level;		// 00
		u8	pad02[6];	// 02
		TESForm	* form;	// 08
		u16	count;		// 10
		u8	pad12[6];	// 12
	};

	BSSimpleList <Entry *> list;	// 08
	u8	chanceNone;	// 18
	u8	flags;		// 19
	u8	pad1A[6];	// 1A
};

static_assert(sizeof(TESLeveledList) == 0x20);
static_assert(sizeof(TESLeveledList::Entry) == 0x18);

// 68
class TESLevItem : public TESBoundObject
{
public:
	virtual ~TESLevItem();

	TESLeveledList	leveledList;	// 48
};

static_assert(sizeof(TESLevItem) == 0x68);

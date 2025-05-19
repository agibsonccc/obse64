#pragma once

#include "NiTypes.h"
#include "GameForms.h"
#include "GameBSExtraData.h"
#include "AltarSync.h"

class NiNode;

// 8
class TESChildCell
{
public:
	virtual TESObjectCELL * GetChildCell();

//	void	** _vtbl;	// 0
};

// 138
class TESObjectREFR : public TESForm
{
public:
	TESObjectREFR();
	virtual ~TESObjectREFR();

	virtual void Unk_3C();
	virtual void Unk_3D();
	virtual void Unk_3E();
	virtual void Unk_3F();
	virtual void Unk_40();
	virtual void Unk_41();
	virtual void Unk_42();
	virtual void Unk_43();
	virtual void Unk_44();
	virtual void Unk_45();
	virtual void Unk_46();
	virtual void Unk_47();
	virtual void Unk_48();
	virtual void Unk_49();
	virtual void Unk_4A();
	virtual void Unk_4B();
	virtual void Unk_4C();
	virtual void Unk_4D();
	virtual void Unk_4E();
	virtual void Unk_4F();
	virtual void Unk_50();
	virtual void Unk_51();
	virtual void Unk_52();
	virtual void Unk_53();
	virtual void Unk_54();
	virtual void Unk_55();
	virtual void Unk_56();
	virtual void Unk_57();
	virtual void Unk_58();
	virtual void Unk_59();
	virtual void Unk_5A();
	virtual void Unk_5B();
	virtual void Unk_5C();
	virtual void Unk_5D();
	virtual void Unk_5E();
	virtual void Unk_5F();
	virtual void Unk_60();
	virtual void Unk_61();
	virtual TESForm * GetBaseForm();
	virtual void Unk_63();
	virtual void Unk_64();
	virtual void Unk_65();
	virtual void Unk_66();
	virtual void Unk_67();
	virtual void Unk_68();
	virtual void Unk_69();
	virtual void Unk_6A();
	virtual void Unk_6B();
	virtual void Unk_6C();
	virtual void Unk_6D();
	virtual void Unk_6E();
	virtual void Unk_6F();
	virtual void Unk_70();
	virtual void Unk_71();
	virtual void Unk_72();
	virtual void Unk_73();
	virtual void Unk_74();
	virtual void Unk_75();
	virtual void Unk_76();

	TESChildCell	childCell;		// 30
	IVPairableItem	pairableItem;	// 38 new in altar

	TESForm			* baseForm;		// 50
	NiPoint3		rotation;		// 58
	NiPoint3		position;		// 64
	float			scale;			// 70
	u32				pad74;			// 74
	NiNode			* niNode;		// 78
	TESObjectCELL	* parentCell;	// 80
	BaseExtraList	extraList;		// 88

	// new in altar
	u8	unkA8[0x138 - 0x0A8];
};

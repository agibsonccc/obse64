#pragma once

#include "GameBSExtraData.h"
#include "GameTypes.h"

class TESForm;
class TESObjectREFR;
class ExtraDataList;

// 20
class ExtraContainerChanges : public BSExtraData
{
public:
	enum { kID = kExtraData_ContainerChanges };

	// 18
	struct Entry
	{
		BSSimpleList <ExtraDataList *>	* extraData;	// 00

		s32	countDelta;	// 08
		u32	pad0C;		// 0C
		TESForm	* type;	// 10
	};

	// 18
	struct Data
	{
		BSSimpleList <Entry>	* objList;	// 00
		TESObjectREFR			* owner;	// 08

		float	totalWeight;	// 10 of all items in inventory. cached, is -1 if needs to be recalculated
		float	armorWeight;	// 14 weight of equipped armor. cached as above. both take armor weight perks into account
	};

	Data	* data;	// 18

	BSSimpleList <Entry> * GetObjList() const { return data ? data->objList : nullptr; }
};

static_assert(sizeof(ExtraContainerChanges) == 0x20);
static_assert(sizeof(ExtraContainerChanges::Data) == 0x18);
static_assert(sizeof(ExtraContainerChanges::Entry) == 0x18);

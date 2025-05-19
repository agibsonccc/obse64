#include "GameScript.h"
#include "GameBSExtraData.h"
#include "GameExtraData.h"
#include "GameObjects.h"
#include "GameRTTI.h"
#include "ParamInfos.h"
#include <vector>
#include <map>

typedef std::vector <ExtraContainerChanges::Entry *> ExtraDataVec;
typedef std::map <TESForm *, size_t> ExtraContainerMap;

// unpack the linked list in to a vector/map
class ExtraContainerInfo
{
public:
	ExtraContainerInfo(BSSimpleList <ExtraContainerChanges::Entry *> * entryList)
	{
		m_vec.reserve(128);

		if(entryList)
		{
			for(auto * iter = &entryList->node; iter; iter = iter->m_next)
			{
				Accept(iter->m_data);
			}
		}
	}

	bool Accept(ExtraContainerChanges::Entry * data)
	{
		if(data)
		{
			m_vec.push_back(data);
			m_map[data->type] = m_vec.size() - 1;
		}

		return true;
	}

	bool IsValidContainerData(TESContainer::Entry * containerData, s32 & numObjects)
	{
		if(containerData) {
			numObjects = containerData->count;

			// make sure it isn't a leveled object
			if(DYNAMIC_CAST(containerData->type, TESForm, TESLevItem))
				return false;

			auto it = m_map.find(containerData->type);
			if(it != m_map.end()) {
				size_t index = it->second;
				if(auto extraData = m_vec[index])
				{
					numObjects += extraData->countDelta;

					// let's clear this item from the vector
					// this way we don't bother to look for it in the second step
					m_vec[index] = nullptr;
				}
			}

			// is at least one still here?
			if(numObjects > 0)
				return true;
		}
		return false;
	}

	ExtraDataVec		m_vec;
	ExtraContainerMap	m_map;
};

static TESForm * GetItemByIdx(TESObjectREFR * thisObj, u32 objIdx, s32 * outNumItems)
{
	if(!thisObj) return nullptr;

	u32 count = 0;

	auto * containerChanges = thisObj->extraList.Get<ExtraContainerChanges>();
	ExtraContainerInfo info(containerChanges ? containerChanges->GetObjList() : nullptr);

	TESContainer * container = nullptr;
	TESForm * baseForm = thisObj->GetBaseForm();
	if(baseForm)
		container = DYNAMIC_CAST(baseForm, TESForm, TESContainer);

	if(container)
	{
		for(auto * iter = &container->list.node; iter; iter = iter->m_next)
		{
			s32 numObjects = 0;
			if(info.IsValidContainerData(iter->m_data, numObjects))
			{
				if(count == objIdx)
				{
					if(outNumItems) *outNumItems = numObjects;
					return iter->m_data->type;
				}
				count++;
			}
		}
	}

	for(auto iter = info.m_vec.begin(); iter != info.m_vec.end(); ++iter)
	{
		auto * extraData = *iter;
		if(extraData && extraData->countDelta > 0)
		{
			if(count == objIdx)
			{
				if(outNumItems) *outNumItems = extraData->countDelta;
				return extraData->type;
			}
			count++;
		}
	}

	if(outNumItems) *outNumItems = 0;

	return nullptr;
}

bool Cmd_GetNumItems_Execute(COMMAND_ARGS)
{
	*result = 0;

	BaseExtraList::Locker lock;

	auto * containerChanges = thisObj->extraList.Get<ExtraContainerChanges>();
	ExtraContainerInfo info(containerChanges ? containerChanges->GetObjList() : nullptr);

	TESContainer * container = nullptr;
	TESForm * baseForm = thisObj->GetBaseForm();
	if(baseForm)
		container = DYNAMIC_CAST(baseForm, TESForm, TESContainer);

	u32 count = 0;

	if(container)
	{
		for(auto * iter = &container->list.node; iter; iter = iter->m_next)
		{
			s32 numObjects = 0;
			if(info.IsValidContainerData(iter->m_data, numObjects))
				count++;
		}
	}

	for(auto iter = info.m_vec.begin(); iter != info.m_vec.end(); ++iter)
	{
		auto * extraData = *iter;
		if(extraData && extraData->countDelta > 0)
			count++;
	}

	*result = count;

	return true;
}

bool Cmd_GetInventoryItemType_Execute(COMMAND_ARGS)
{
	*result = 0;

	if(!thisObj) return true;

	u32 objIdx = 0;
	if(!ExtractArgs(EXTRACT_ARGS, &objIdx)) return true;

	BaseExtraList::Locker lock;

	if(TESForm * type = GetItemByIdx(thisObj, objIdx, nullptr))
		*((u32 *)result) = type->refID;

	return true;
}

CommandInfo kCommandInfo_GetNumItems =
{
	"GetNumItems", "gni",
	0,
	"return the number of items in an object's inventory",
	1,
	0, nullptr,
	Cmd_GetNumItems_Execute,
};

CommandInfo kCommandInfo_GetInventoryItemType =
{
	"GetInventoryItemType", "giit",
	0,
	"returns a ref to the type of the inventory item",
	1,
	1, kParams_OneInt,
	Cmd_GetInventoryItemType_Execute
};

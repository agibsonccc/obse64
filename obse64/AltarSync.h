#pragma once

#include "UnrealObject.h"
#include "UnrealTypes.h"

class UTESForm;

// 178
class UTESSync : public UObject
{
public:
	TMap <u32, FString>		m_allForms;						// 28 uproperty
	TMap <FString, u8>		m_fileCompileIndexes;			// 78 uproperty
	TMap <FString, u8>		m_filedRuntimeCompileIndexes;	// C8 uproperty
	TMap <u32, UTESForm *>	m_dynamicForms;					// 118 uproperty
	TArray <void *>			m_unk;							// 168 members need virtual destruction, probably uobject?
};

static_assert(sizeof(UTESSync) == 0x178);

// 10
struct VAltarActorGUID
{
	// just a standard guid?
	u32	a;
	u32	b;
	u32	c;
	u32	d;
};

static_assert(sizeof(VAltarActorGUID) == 0x10);

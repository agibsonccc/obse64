#include "UnrealTypes.h"

RelocAddr <FNameStorage> g_fnameStorage(0x0909EE80);

FNameEntry * FNameStorage::Get(u32 idx)
{
	u32	entry = (idx >> 16) & 0xFFFF;	// chunk offset
	u32 offset = (idx & 0xFFFF) * 4;	// byte offset

	u8 * buf = (u8 *)m_chunks[entry];

	return (FNameEntry *)(buf + offset);
}

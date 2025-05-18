#include "GameBSExtraData.h"
#include "obse64_common/Relocation.h"
#include <Windows.h>

RelocPtr <CRITICAL_SECTION> g_extraListCS(0x094B5B80);

void BaseExtraList::Lock()
{
	EnterCriticalSection(g_extraListCS);
}

void BaseExtraList::Unlock()
{
	LeaveCriticalSection(g_extraListCS);
}

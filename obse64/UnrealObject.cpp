#include "UnrealObject.h"
#include <Windows.h>

void FUObjectArray::Lock()
{
	EnterCriticalSection((CRITICAL_SECTION *)&m_lock);
}

void FUObjectArray::Unlock()
{
	LeaveCriticalSection((CRITICAL_SECTION *)&m_lock);
}

RelocPtr <FUObjectArray> g_uObjectArray(0x09145160);

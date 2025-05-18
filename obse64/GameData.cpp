#include "GameData.h"

RelocPtr <TESDataHandler *> g_dataHandler(0x094B5C98);

TESDataHandler * TESDataHandler::GetSingleton()
{
	return *g_dataHandler;
}

#include "GameScript.h"
#include "ParamInfos.h"
#include <Windows.h>

// deprecated

static bool Cmd_IsKeyPressed_Execute(COMMAND_ARGS)
{
	*result = 0;
	u32 keycode = 0;

	if(!ExtractArgs(EXTRACT_ARGS, &keycode)) return true;
	if(GetAsyncKeyState(keycode) & 0x8000) *result = 1;

	return true;
}

CommandInfo kCommandInfo_IsKeyPressed =
{
	"IsKeyPressed", "ikp",
	0,
	"return if a virtual keycode is down or up",
	0,
	1, kParams_OneInt,
	Cmd_IsKeyPressed_Execute
};

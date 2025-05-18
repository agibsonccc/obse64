#include "CommandTable.h"
#include "GameScript.h"
#include "Hooks_Script.h"
#include "obse64_common/Log.h"
#include "GameConsole.h"

#define HANDLER(x) x

bool Cmd_Test_Execute(COMMAND_ARGS)
{
	return true;
}

bool Cmd_TestArgs_Execute(COMMAND_ARGS)
{
	u32	arg;

	if(ExtractArgs(EXTRACT_ARGS, &arg))
	{
		Console_Print("testargcommand: %d", arg);
	}
	else
	{
		Console_Print("testargcommand: couldn't extract args");
	}

	return true;
}

static CommandInfo kTestCommand =
{
	"testcommand", "tcmd",
	0,
	"test command for obse",
	0,
	0, nullptr,

	HANDLER(Cmd_Test_Execute)
};

static ParamInfo kTestArgCommand_Params[] =
{
	{	"int", kParamType_Integer, 0 }
};

static CommandInfo kTestArgCommand =
{
	"testargcommand", "tacmd",
	0,
	"test argument command for obse",
	0,
	1, kTestArgCommand_Params,

	HANDLER(Cmd_TestArgs_Execute)
};

void AddScriptCommands()
{
	AddScriptCommand(kTestCommand);
	AddScriptCommand(kTestArgCommand);
}

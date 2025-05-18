#include "GameScript.h"

RelocAddr <ParseFunction> DefaultParseFunction(0x06972980);
RelocAddr <_ExtractArgs> ExtractArgs(0x066598E0);

// wrap this to give it a static address
bool Cmd_Default_Parse(u32 numParams, const ParamInfo * paramInfo, ScriptLineBuffer * lineBuf, ScriptBuffer * scriptBuf)
{
	return DefaultParseFunction(numParams, paramInfo, lineBuf, scriptBuf);
}

RelocPtr <CommandInfo>	g_firstScriptCommand(0x08FEC850);
RelocPtr <CommandInfo>	g_firstConsoleCommand(0x08FF3C40);

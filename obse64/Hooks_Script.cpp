#include "Hooks_Script.h"
#include "obse64/GameConsole.h"
#include "obse64/GameScript.h"
#include "obse64_common/SafeWrite.h"
#include "obse64_common/obse64_version.h"
#include "obse64_common/BranchTrampoline.h"
#include "obse64_common/Relocation.h"
#include "obse64_common/Log.h"
#include "obse64_common/Errors.h"
#include "xbyak/xbyak.h"
#include <vector>

RelocAddr <ParseFunction> DefaultParseFunction(0x06972980);

bool PadCommand_Execute(const ParamInfo * paramInfo,
	const char * scriptData,
	TESObjectREFR * thisObj,
	TESObjectREFR * containingObj,
	Script * script,
	ScriptLocals * locals,
	float * result,
	u32 * opcodeOffsetPtr)
{
	return true;
}

bool GetOBSEVersion_Execute(const ParamInfo * paramInfo,
	const char * scriptData,
	TESObjectREFR * thisObj,
	TESObjectREFR * containingObj,
	Script * script,
	ScriptLocals * locals,
	float * result,
	u32 * opcodeOffsetPtr)
{
	_MESSAGE("GetOBSEVersion_Execute");

	Console_Print("OBSE64 version: %d.%d.%d, release idx %d, runtime %08X",
		OBSE_VERSION_INTEGER, OBSE_VERSION_INTEGER_MINOR, OBSE_VERSION_INTEGER_BETA,
		OBSE_VERSION_RELEASEIDX, RUNTIME_VERSION);

	return true;
}

void ConsoleCommandInit_Hook()
{
	for (CommandInfo * iter = g_firstConsoleCommand; iter->opcode < (kScript_NumConsoleCommands + kScript_ConsoleOpBase); ++iter)
	{
		if (!iter->execute)
			continue;

		if (!strcmp(iter->name, "WasteMemory"))
		{
			CommandInfo & cmd = *iter;

			cmd.name = "GetOBSEVersion";
			cmd.shortName = "";
			cmd.helpText = "";
			cmd.needsParent = true;
			cmd.numParams = 0;
			cmd.execute = GetOBSEVersion_Execute;
			cmd.editorFilter = false;
			cmd.invalidatesCellList = false;

			break;
		}
	}
}

class HookedCommandTable
{
public:
	// duplicate the command table locally
	void	Init(u32 baseOpcode, const CommandInfo * ptr, u32 num);
	
	// pad out to new opcode
	void	Extend(u32 opcode);

	struct PatchInfo
	{
		u32	ptr;
		u32	offset;
		u32	type;
	};

	// copy to trampoline and apply code patches
	void	Apply(const PatchInfo * start, const PatchInfo * end, const PatchInfo * len);

private:
	std::vector <CommandInfo>	m_commands;
	u32	m_baseOpcode;

	CommandInfo	* m_trampolineCopy = nullptr;
};

HookedCommandTable g_commandTable;

void HookedCommandTable::Init(u32 baseOpcode, const CommandInfo * ptr, u32 num)
{
	m_baseOpcode = baseOpcode;

	m_commands.assign(ptr, ptr + num);
}

void HookedCommandTable::Extend(u32 opcode)
{
	u32 lastOpcode = m_commands.back().opcode;

	CommandInfo pad =
	{
		"", "",
		0, 0,
		"empty padding cmd",
		false, 0,
		0, 0,
		nullptr,

		PadCommand_Execute,
		DefaultParseFunction,
		nullptr,

		false, false
	};

	u32 commandsToAdd = opcode - lastOpcode + 1;
	m_commands.reserve(m_commands.size() + commandsToAdd);

	for(u32 i = lastOpcode + 1; i < opcode; i++)
	{
		pad.opcode = i;
		m_commands.push_back(pad);
	}
}

void HookedCommandTable::Apply(const PatchInfo * start, const PatchInfo * end, const PatchInfo * len)
{
	// game code references the command table via many 32-bit displacements
	// so it needs to go in the branch trampoline

	size_t tableSize = m_commands.size() * sizeof(m_commands[0]);

	_MESSAGE("command table size: %016I64X", tableSize);

	m_trampolineCopy = (CommandInfo *)g_branchTrampoline.allocate(tableSize);
	ASSERT(m_trampolineCopy);

	std::copy(m_commands.begin(), m_commands.end(), m_trampolineCopy);
}

void Hooks_Script_Apply()
{
	g_commandTable.Init(kScript_ScriptOpBase, g_firstScriptCommand, kScript_NumScriptCommands);
	g_commandTable.Extend(kScript_OBSEOpBase);

	ConsoleCommandInit_Hook();
}

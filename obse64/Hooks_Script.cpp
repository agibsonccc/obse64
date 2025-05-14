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

// 8
struct ParamTypeInfo
{
	u32		typeID;		// 0
	bool	variable;	// 4
	bool	refType;	// 5
	u8		pad6[2];	// 6
};

// may need to patch/redirect this as well
RelocPtr <ParamTypeInfo> g_paramTypeInfo(0x08FF6670);

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

	// number of commands registered (includes padding, vanilla, extended)
	size_t		NumCommands() const { return m_commands.size(); }

	CommandInfo *	GetByIdx(u32 idx) { return &m_commands[idx]; }
	CommandInfo *	GetByOpcode(u32 opcode) { return GetByIdx(opcode - m_baseOpcode); }

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

enum CmdTablePatchType
{
	kPatchType_End = 0,

	// write two bytes
	kPatchType_Data16,

	// write four bytes
	kPatchType_Data32,

	// 7 byte "lea r64, offset32" instruction
	// will be converted to a 7 byte "mov r64, offset32" instruction
	// offset will be redirected to a pointer stored in the trampoline
	// different values of the 'offset' struct field will add and reference additional pointers
	kPatchType_LeaToLoadBase,

	// used to clear offset (instruction not parsed)
	// 8 byte "mov r9, offset[rbx + r9*8]" for example
	// 7 byte "mov eax, offset[rax + rcx*8]" also for example
	// or 
	kPatchType_WriteOffset32,

	// used when fetches of imagebase are used for multiple things
	// but the extra things are unused/debug only
	// must point at an 8 byte mov statement
	// "mov rbx, offset32[rbx + rcx*8]" changed to
	// "lea rbx, offset32; nop"
	kPatchType_FixupStringFetch
};

HookedCommandTable::PatchInfo kCmdTableStartPatches[] =
{
	// +00
	{ 0x0696E720 + 0x0D1D,		0x00, kPatchType_LeaToLoadBase },

	{ 0x0696FE00 + 0x0033,		0x00, kPatchType_LeaToLoadBase },

	{ 0x06972140 + 0x04C2,		0x00, kPatchType_LeaToLoadBase },

	{ 0x0698EDB0 + 0x0184,		0x00, kPatchType_LeaToLoadBase },	// this register is reused at +2DA, which is a debug-only string
	{ 0x0698EDB0 + 0x018B + 4,	0x00, kPatchType_WriteOffset32 },
	{ 0x0698EDB0 + 0x02DA,		0x00, kPatchType_FixupStringFetch },

	{ 0x0698F100 + 0x006C,		0x00, kPatchType_LeaToLoadBase },

	{ 0x06A35210 + 0x009B,		0x00, kPatchType_LeaToLoadBase },

	{ 0x06A70820 + 0x000D,		0x00, kPatchType_LeaToLoadBase },

	// do not patch 0x0696FC80 + 0x0025, used as end pointer for search over keywords
	
	{ 0x0696FC80 + 0x0066,		0x08, kPatchType_LeaToLoadBase },

	{ 0x0696FC80 + 0x0122,		0x10, kPatchType_LeaToLoadBase },
	{ 0x0696FC80 + 0x0129 + 3,	0x00, kPatchType_WriteOffset32 },

	{ 0x06A70800 + 0x0011,		0x22, kPatchType_LeaToLoadBase },

//	{ 0x06A70870 + 0x000A,		0x00, kPatchType_LeaToLoadBase },	// used to read multiple offsets, use base 0 for simplicity
//	{ 0x06A70870 + 0x0018 + 5,	0x22, kPatchType_WriteOffset32 },
//	{ 0x06A70870 + 0x0025 + 4,	0x28, kPatchType_WriteOffset32 },
	// 0x06A70870 + 0x0037 needs special handling because it's reading a different array
	// just reimplement 06A70870, see IsScriptCmdParamAReference

	{ 0, 0, kPatchType_End }
};

HookedCommandTable::PatchInfo kCmdTableEndPatches[] =
{
	{ 0x0696FC80 + 0x006D,		8, kPatchType_LeaToLoadBase },

	{ 0, 0, kPatchType_End }
};

HookedCommandTable::PatchInfo kCmdTableLenPatches[] =
{
	{ 0x0696FE00 + 0x0028 + 2, u32(-1), kPatchType_Data32 },

	{ 0x06A35210 + 0x00A2 + 2,	0, kPatchType_Data32 },

	{ 0x06A70800 + 0x0000 + 2,	0, kPatchType_Data32 },

	{ 0x06A70820 + 0x0003 + 2,	0, kPatchType_Data32 },

//	{ 0x06A70870 + 0x0000 + 2,	0, kPatchType_Data32 },

	{ 0, 0, kPatchType_End }
};

void HookedCommandTable::Apply(const PatchInfo * start, const PatchInfo * end, const PatchInfo * len)
{
	// todo: see if all of these can be replaced with reading pointers from the trampoline
	// todo: or code hooks so the table can be dynamic (and not in the trampoline)
	
	// game code references the command table via many 32-bit displacements
	// so it needs to go in the branch trampoline

	size_t tableSize = m_commands.size() * sizeof(m_commands[0]);

	_MESSAGE("command table size: %016I64X", tableSize);

	m_trampolineCopy = (CommandInfo *)g_branchTrampoline.allocate(tableSize);
	ASSERT(m_trampolineCopy);

	std::copy(m_commands.begin(), m_commands.end(), m_trampolineCopy);
}

bool IsScriptCmdParamAReference(u32 scriptCmdIdx, u32 paramTypeIdx)
{
	auto & cmdTable = g_commandTable;

	if(scriptCmdIdx >= cmdTable.NumCommands())
		return false;

	const auto * cmd = cmdTable.GetByIdx(scriptCmdIdx);
	if(paramTypeIdx >= cmd->numParams)
		return false;

	u32 typeID = cmd->params[paramTypeIdx].typeID;

	return g_paramTypeInfo[typeID].refType;
}

RelocAddr <decltype(&IsScriptCmdParamAReference)> IsScriptCmdParamAReference_Original(0x06A70870);

void Hooks_Script_Apply()
{
	g_commandTable.Init(kScript_ScriptOpBase, g_firstScriptCommand, kScript_NumScriptCommands);
	g_commandTable.Extend(kScript_OBSEOpBase);

	g_branchTrampoline.write6Branch(IsScriptCmdParamAReference_Original.getUIntPtr(), uintptr_t(IsScriptCmdParamAReference));

	ConsoleCommandInit_Hook();
}

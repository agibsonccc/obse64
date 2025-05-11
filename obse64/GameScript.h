#pragma once

#include "obse64_common/Relocation.h"
#include "obse64_common/Utilities.h"

class TESForm;
class TESObjectREFR;
class TESQuest;

class Script;

struct ScriptLocals;
struct ScriptLineBuffer;
struct ScriptBuffer;

// 10
struct ParamInfo
{
	const char	* typeStr;		// 00
	u32			typeID;			// 08
	bool		isOptional;		// 0C
	u8			pad0D[3];		// 0D
};

static_assert(sizeof(ParamInfo) == 0x10);

typedef bool (* ExecuteFunction)(
	const ParamInfo * paramInfo,
	const char * scriptData,
	TESObjectREFR * thisObj,
	TESObjectREFR * containingObj,
	Script * script,
	ScriptLocals * locals,
	float * result,
	u32 * opcodeOffsetPtr);

typedef bool (* ParseFunction)(
	u32 numParams,
	const ParamInfo * paramInfo,
	ScriptLineBuffer * lineBuf,
	ScriptBuffer * scriptBuf);

typedef bool (* ConditionFunction)(
	TESObjectREFR * thisObj,
	void * arg1,
	void * arg2,
	double * result);

// 50
struct CommandInfo
{
	const char			* name;					// 00
	const char			* shortName;			// 08
	u32					opcode;					// 10
	u32					pad14;					// 14
	const char			* helpText;				// 18
	bool				needsParent;			// 20
	u8					pad21;					// 21
	u16					numParams;				// 22
	u32					pad24;					// 24
	ParamInfo			* params;				// 28

	// handlers
	ExecuteFunction		execute;				// 30
	ParseFunction		parse;					// 38
	ConditionFunction	condition;				// 40

	// flags
	bool				editorFilter;			// 48
	bool				invalidatesCellList;	// 49
	u8					pad4A[6];				// 4A
};

static_assert(sizeof(CommandInfo) == 0x50);

enum
{
	kScript_NumScriptCommands = 0x0172,
	kScript_NumConsoleCommands = 0x0086,

	kScript_ScriptOpBase = 0x1000,
	kScript_ConsoleOpBase = 0x0100,

	kScript_OBSEOpBase = 0x1400,
};

extern RelocPtr <CommandInfo>	g_firstScriptCommand;
extern RelocPtr <CommandInfo>	g_firstConsoleCommand;

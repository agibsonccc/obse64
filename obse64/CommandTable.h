#pragma once

void AddScriptCommands();

#define COMMAND_ARGS		const ParamInfo * paramInfo, const char * scriptData, TESObjectREFR * thisObj, TESObjectREFR * containingObj, Script * script, ScriptLocals * locals, float * result, u32 * opcodeOffsetPtr
#define PASS_COMMAND_ARGS	paramInfo, scriptData, thisObj, containingObj, script, locals, result, opcodeOffsetPtr
#define EXTRACT_ARGS		paramInfo, scriptData, opcodeOffsetPtr, thisObj, containingObj, script, locals
#define COMMAND_ARGS_EVAL	TESObjectREFR * thisObj, void * arg1, void * arg2, double * result

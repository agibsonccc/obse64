#include "GameScript.h"
#include "GameObjects.h"
#include "GameConsole.h"
#include "GameRTTI.h"

class MagicItem;

static bool Cmd_GetActiveSpell_Execute(COMMAND_ARGS)
{
	u32 * refResult = (u32 *)result;
	MagicItem * activeMagicItem = CALL_MEMBER_FN(PlayerCharacter::Get(), GetActiveMagicItem)();

	*refResult = 0;

	auto * activeMagicItemForm = DYNAMIC_CAST(activeMagicItem, MagicItem, TESForm);
	if(activeMagicItemForm)
		*refResult = activeMagicItemForm->refID;

	if(IsConsoleMode())
	{
		Console_Print("GetActiveSpell >> %08X (%s)", *refResult, GetFullName(activeMagicItemForm));
	}

	return true;
}

CommandInfo kCommandInfo_GetActiveSpell =
{
	"GetPlayerSpell", "GetActiveSpell",
	0,
	"returns the base spell object for the player's active spell",
	0,
	0, nullptr,
	Cmd_GetActiveSpell_Execute
};

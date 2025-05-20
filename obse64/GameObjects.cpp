#include "GameObjects.h"
#include "obse64_common/Relocation.h"

RelocAddr <PlayerCharacter *> PlayerCharacterSingleton(0x09455388);

PlayerCharacter * PlayerCharacter::Get()
{
	return PlayerCharacterSingleton;
}

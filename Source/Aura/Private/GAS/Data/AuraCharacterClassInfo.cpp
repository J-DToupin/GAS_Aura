// Copyright JDgame


#include "GAS/Data/AuraCharacterClassInfo.h"

FCharacterClassDefaultInfo UAuraCharacterClassInfo::GetClassDefaultInfo(const ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}

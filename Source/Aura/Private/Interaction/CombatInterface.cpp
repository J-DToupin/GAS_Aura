// Copyright JDgame


#include "Interaction/CombatInterface.h"


// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetCharacterLevel()
{
	return 0;
}

FVector ICombatInterface::GetCombatSocketLocation()
{
	return FVector();
}

FVector ICombatInterface::GetMouseLocation()
{
	return FVector();
}

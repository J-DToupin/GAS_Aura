// Copyright JDgame


#include "..\..\..\Public\GAS\Data\AuraLevelUpInfo.h"

int32 UAuraLevelUpInfo::FindLevelByXp(const int32 AmountXp) const
{
	for (int Index = 1; Index < LevelUpInfos.Num(); ++Index)
	{
		if (AmountXp == LevelUpInfos[Index].LevelUpRequirement)
		{
			return Index + 1;
		}
		
		if (AmountXp < LevelUpInfos[Index].LevelUpRequirement)
		{
			return Index;
		}
	}

	return 0;
}

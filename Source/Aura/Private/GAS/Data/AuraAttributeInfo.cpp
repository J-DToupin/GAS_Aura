﻿// Copyright JDgame


#include "GAS/Data/AuraAttributeInfo.h"

#include "Aura/AuraLogChannels.h"

FAttributeInfo UAuraAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,
                                                           bool bLogNotFound) const
{
	
	for (const FAttributeInfo Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAttributeInfo{};
}

﻿// Copyright JDgame


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "GAS/AuraAttributeSet.h"
#include "AuraGameplayTags.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		check(AttributeInfo)
		for (FAttributeInfo Info : AttributeInfo->AttributeInformation)
		{
			if (FGameplayAttribute (*const GetGameplayAttributePointer)() = *AuraAttributeSet->TagsToAttributes.Find(Info.AttributeTag))
			{
				Info.AttributeValue = GetGameplayAttributePointer().GetNumericValue(AttributeSet);
				OnAttributeInfoChangeDelegate.Broadcast(Info);
			}
			
		}
	}
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		for (auto& PairTagAttributes : AuraAttributeSet->TagsToAttributes)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				PairTagAttributes.Value()).AddLambda(
					[this, PairTagAttributes](const FOnAttributeChangeData& Data)
					{
						BroadcastAttributeInfo(PairTagAttributes.Key, PairTagAttributes.Value());
					});
		}
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag, const FGameplayAttribute& GameplayAttribute) const
{
	FAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(GameplayTag);
	Info.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	OnAttributeInfoChangeDelegate.Broadcast(Info);
}

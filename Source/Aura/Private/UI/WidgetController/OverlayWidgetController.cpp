// Copyright JDgame


#include "UI/WidgetController/OverlayWidgetController.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChangeDelegate.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChangeDelegate.Broadcast(AuraAttributeSet->GetMaxHealth());
		
		OnManaChangeDelegate.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChangeDelegate.Broadcast(AuraAttributeSet->GetMaxMana());
	}
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChangeDelegate.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChangeDelegate.Broadcast(Data.NewValue);
		});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnManaChangeDelegate.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChangeDelegate.Broadcast(Data.NewValue);
		});
	}

	// utilise un Lambda function [on met this pour que la function est acess au function de cette classe]
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTagDelegate.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				//"A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
				if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
				{
					const FUIWidgetRow* WidgetRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*WidgetRow);
				}
			}
		}
	);
	
}
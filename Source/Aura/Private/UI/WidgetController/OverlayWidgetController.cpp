// Copyright JDgame


#include "UI/WidgetController/OverlayWidgetController.h"

#include "Character/Player/AuraPlayerState.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"
#include "GAS/Data/AbilityInfo.h"
#include "GAS/Data/AuraLevelUpInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChangeDelegate.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChangeDelegate.Broadcast(AuraAttributeSet->GetMaxHealth());
		
		OnManaChangeDelegate.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChangeDelegate.Broadcast(AuraAttributeSet->GetMaxMana());
	}

	if (const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
	{
		OnXpChangeDelegate.Broadcast(AuraPlayerState->GetCharacterExperience());
		OnPlayerLevelChangeDelegate.Broadcast(AuraPlayerState->GetCharacterLevel());
	}
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
	{
		AuraPlayerState->OnChangeXpDelegate.AddUObject(this, &UOverlayWidgetController::OnXpChanged);
		AuraPlayerState->OnChangeLevelDelegate.AddLambda([this](int32 NewLevel)
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
		});
	}

	
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
		
		if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
		{
			if (AuraAbilitySystemComponent->bStartupAbilitiesGiven)
			{
				OnInitializeStartupAbility(AuraAbilitySystemComponent);
			}
			else
			{
				AuraAbilitySystemComponent->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::OnInitializeStartupAbility);
			}
			
			// utilise un Lambda function [on met this pour que la function est acess au function de cette classe]
			AuraAbilitySystemComponent->EffectAssetTagDelegate.AddLambda(
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
	}
	
}

void UOverlayWidgetController::OnInitializeStartupAbility(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	//TODO get information about all gi ven abilities

	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		
		FAuraAbilityInfo AuraAbilityInfo = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		AuraAbilityInfo.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);

		AbilityInfoDelegate.Broadcast(AuraAbilityInfo);
	});

	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXpChanged(const int32 AmountXp)
{
	if (const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
	{
		const UAuraLevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
		checkf(LevelUpInfo, TEXT("LevelUpInfo not Setup in AuraPlayerState"))

		const int32 IndexLevel = LevelUpInfo->FindLevelByXp(AmountXp);
		const int32 MaxLevel = LevelUpInfo->LevelUpInfos.Num();

		if (IndexLevel <= MaxLevel && IndexLevel > 0)
		{
			
			const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInfos[IndexLevel - 1].LevelUpRequirement;
			
			const int32 LevelUpRequirement = LevelUpInfo->LevelUpInfos[IndexLevel].LevelUpRequirement;
			
			const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
			const int32 XPForThisLevel = AmountXp - PreviousLevelUpRequirement;

			if (DeltaLevelRequirement > 0)
			{
				const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
				OnXpChangeDelegate.Broadcast(XPBarPercent);
			}
			
		}
	}
}

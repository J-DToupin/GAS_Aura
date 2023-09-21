// Copyright JDgame


#include "GAS/AuraAbilitySystemComponent.h"


// Sets default values for this component's properties
UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
	
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}


// Called when the game starts
void UAuraAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTagDelegate.Broadcast(TagContainer);
	
}


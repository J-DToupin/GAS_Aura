// Copyright JDgame


#include "GAS/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"


// Sets default values for this component's properties
UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
	
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	const FString TestGameplayTags = GameplayTags.Attributes_Secondary_Armor.ToString();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TestGameplayTags);
	
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


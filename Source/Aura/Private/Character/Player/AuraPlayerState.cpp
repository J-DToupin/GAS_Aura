// Copyright JDgame


#include "Character/Player/AuraPlayerState.h"

#include "AbilitySystemComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	// vitese de replicad
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilityComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, CharacterLevel);
	DOREPLIFETIME(AAuraPlayerState, CharacterExperience);
	DOREPLIFETIME(AAuraPlayerState, AttributePoint);
	DOREPLIFETIME(AAuraPlayerState, SpellPoint);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

int32 AAuraPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 AAuraPlayerState::GetCharacterExperience() const
{
	return CharacterExperience;
}

int32 AAuraPlayerState::GetAttributePoint() const
{
	return AttributePoint;
}

int32 AAuraPlayerState::GetSpellPoint() const
{
	return SpellPoint;
}

void AAuraPlayerState::AddToXP(const int32 AmountXp)
{
	CharacterExperience += AmountXp;
	OnChangeXpDelegate.Broadcast(CharacterExperience);
}

void AAuraPlayerState::AddToLevel(const int32 AmountLevel)
{
	CharacterLevel += AmountLevel;
	OnChangeLevelDelegate.Broadcast(CharacterLevel);
}

void AAuraPlayerState::AddToAttributePoint(const int32 AmountAttributePoint)
{
	AttributePoint += AmountAttributePoint;
	OnChangeAttributePointDelegate.Broadcast(AttributePoint);
}

void AAuraPlayerState::AddToSpellPoint(const int32 AmountSpellPoint)
{
	SpellPoint += AmountSpellPoint;
	OnChangeSpellPointDelegate.Broadcast(SpellPoint);
}

void AAuraPlayerState::SetLevel(const int32 NewLevel)
{
	CharacterLevel = NewLevel;
	OnChangeLevelDelegate.Broadcast(CharacterLevel);
}


void AAuraPlayerState::SetXP(const int32 NewXp)
{
	CharacterExperience = NewXp;
	OnChangeXpDelegate.Broadcast(CharacterExperience);
}

void AAuraPlayerState::SetAttributePoint(const int32 NewAttributePoint)
{
	AttributePoint = NewAttributePoint;
	OnChangeAttributePointDelegate.Broadcast(AttributePoint);
}

void AAuraPlayerState::SetSpellPoint(const int32 NewSpellPoint)
{
	SpellPoint = NewSpellPoint;
	OnChangeSpellPointDelegate.Broadcast(SpellPoint);
}


void AAuraPlayerState::OnRep_CharacterExperience(int32 OLdCharacterExperience)
{
	OnChangeXpDelegate.Broadcast(CharacterExperience);
}

void AAuraPlayerState::OnRep_CharacterLevel(int32 OLdCharacterLevel)
{
	OnChangeLevelDelegate.Broadcast(CharacterLevel);
}

void AAuraPlayerState::OnRep_AttributePoint(int32 OLdAttributePoint)
{
	OnChangeAttributePointDelegate.Broadcast(AttributePoint);
}

void AAuraPlayerState::OnRep_SpellPoint(int32 OLdSpellPoint)
{
	OnChangeSpellPointDelegate.Broadcast(SpellPoint);
}
// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAuraLevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangePlayerStateSignature, int32);

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAuraLevelUpInfo> LevelUpInfo;

	//Getter
	int32 GetCharacterLevel() const;
	int32 GetCharacterExperience() const;
	int32 GetAttributePoint() const;
	int32 GetSpellPoint() const;

	//Add
	UFUNCTION(BlueprintCallable)
	void AddToXP(const int32 AmountXp);
	UFUNCTION(BlueprintCallable)
	void AddToLevel(const int32 AmountLevel);
	UFUNCTION(BlueprintCallable)
	void AddToAttributePoint(const int32 AmountAttributePoint);
	UFUNCTION(BlueprintCallable)
	void AddToSpellPoint(const int32 AmountSpellPoint);

	//Set
	UFUNCTION(BlueprintCallable)
	void SetXP(const int32 NewXp);
	UFUNCTION(BlueprintCallable)
	void SetLevel(const int32 NewLevel);
	UFUNCTION(BlueprintCallable)
	void SetAttributePoint(const int32 NewAttributePoint);
	UFUNCTION(BlueprintCallable)
	void SetSpellPoint(const int32 NewSpellPoint);

	//Delegate
	FOnChangePlayerStateSignature OnChangeXpDelegate;
	FOnChangePlayerStateSignature OnChangeLevelDelegate;
	FOnChangePlayerStateSignature OnChangeAttributePointDelegate;
	FOnChangePlayerStateSignature OnChangeSpellPointDelegate;

protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


private:

	UFUNCTION()
	void OnRep_CharacterLevel(int32 OLdCharacterLevel);

	UFUNCTION()
	void OnRep_AttributePoint(int32 OLdAttributePoint);

	UFUNCTION()
	void OnRep_SpellPoint(int32 OLdSpellPoint);

	UFUNCTION()
	void OnRep_CharacterExperience(int32 OLdCharacterExperience);
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing="OnRep_CharacterLevel")
	int32 CharacterLevel = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing="OnRep_AttributePoint")
	int32 AttributePoint = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing="OnRep_SpellPoint")
	int32 SpellPoint = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing="OnRep_CharacterExperience")
	int32 CharacterExperience = 0;
};

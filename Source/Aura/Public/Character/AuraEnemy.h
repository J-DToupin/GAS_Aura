// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "GAS/Data/AuraCharacterClassInfo.h"
#include "Interaction/TargetInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"


class UWidgetComponent;


UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public ITargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraEnemy();

	// Enemy Interface
	virtual void HighLightActor_Implementation() override;
	virtual void UnHighLightActor_Implementation() override;
	// End Enemy Interface

	// Combat Interface
	virtual int32 GetCharacterLevel() override;

	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();


	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangeSignature OnHealthChangeDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxHealthChangeDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangeSignature OnManaChangeDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxManaChangeDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess), Category="Character")
	int32 CharacterLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess), Category="Character")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
};

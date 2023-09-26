// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/TargetInterface.h"
#include "AuraEnemy.generated.h"

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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess), Category="Character")
	int32 CharacterLevel = 1;
	
};

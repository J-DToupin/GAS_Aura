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

	virtual void HighLightActor_Implementation() override;
	
	virtual void UnHighLightActor_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};

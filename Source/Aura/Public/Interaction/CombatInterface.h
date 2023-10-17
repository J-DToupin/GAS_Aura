﻿// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;
// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual int32 GetCharacterLevel();

	virtual FVector GetCombatSocketLocation();

	virtual FVector GetMouseLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FacingTarget(FVector TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;
};

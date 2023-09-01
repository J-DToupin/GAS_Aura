// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ITargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HighLightActor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnHighLightActor();
};

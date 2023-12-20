// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void AddToXp(int32 InXp);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 NumLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoint(int32 NumAttributePoint);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoint(int32 NumSpellPoint);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetCurrentXp() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(const int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(const int32 Level) const;
	
	UFUNCTION(BlueprintNativeEvent)
	const int32 GetLevelForXp(const int32 TotalXp);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};

// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraLevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelUpRequirement = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointReward = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SpellPointReward = 1;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraLevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	int32 FindLevelByXp(const int32 AmountXp) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLevelUpInfo> LevelUpInfos;
};

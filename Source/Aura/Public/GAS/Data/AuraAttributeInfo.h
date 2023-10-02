// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAttributeInfo.generated.h"


USTRUCT(BlueprintType)
struct FAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttributeTag{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeName{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeDescription{};

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue{};
};
/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	FAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttributeInfo> AttributeInformation;
};

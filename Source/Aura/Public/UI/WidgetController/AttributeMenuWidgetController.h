// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GAS/Data/AuraAttributeInfo.h"
#include "AttributeMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfoChangeSignature, const FAttributeInfo&, AttributeInfo);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeInfoChangeSignature OnAttributeInfoChangeDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Date")
	TObjectPtr<UAuraAttributeInfo> AttributeInfo;

private:
	UFUNCTION()
	void BroadcastAttributeInfo(const FGameplayTag& GameplayTag, const FGameplayAttribute& GameplayAttribute) const;
};

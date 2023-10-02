// Copyright JDgame


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "GAS/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		
	}
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetStrengthAttribute()).AddLambda(
				[](const FOnAttributeChangeData& Data)
				{
					
				});
	}
}

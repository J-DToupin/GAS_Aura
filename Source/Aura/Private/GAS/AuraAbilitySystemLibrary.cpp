// Copyright JDgame


#include "GAS/AuraAbilitySystemLibrary.h"

#include "Character/Player/AuraPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = PC->GetHUD<AAuraHUD>())
		{
			return AuraHUD->GetOverlayWidgetController(GetWidgetControllerParams(WorldContextObject));
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = PC->GetHUD<AAuraHUD>())
		{
			return AuraHUD->GetAttributeMenuWidgetController(GetWidgetControllerParams(WorldContextObject));
		}
	}

	return nullptr;
}

FWidgetControllerParams UAuraAbilitySystemLibrary::GetWidgetControllerParams(const UObject* WorldContextObject)
{
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		const FWidgetControllerParams WidgetControllerParams{PC, PS, ASC, AS};
		return WidgetControllerParams;
	}

	return FWidgetControllerParams{};
}

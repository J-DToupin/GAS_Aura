// Copyright JDgame


#include "UI/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{

	checkf(OverLayWidgetClass, TEXT("OverLay Widget Class uninitialized in BP_AuraHUD"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay WidgetController Class uninitialized in BP_AuraHUD"))
	
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverLayWidgetClass);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* OverlayWidgetC = GetOverlayWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(OverlayWidgetC);
	
	OverlayWidget->AddToViewport();
}

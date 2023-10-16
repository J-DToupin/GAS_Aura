// Copyright JDgame


#include "GAS/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Character/Player/AuraPlayerState.h"
#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	if (const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		AActor* AvatarActor = ASC->GetAvatarActor();

		UAuraCharacterClassInfo* AuraCharacterClassInfo = AuraGameModeBase->CharacterClassInfo;
		check(AuraCharacterClassInfo);
		
		const FCharacterClassDefaultInfo ClassDefaultInfo = AuraCharacterClassInfo->GetClassDefaultInfo(CharacterClass);


		FGameplayEffectContextHandle GameplayEffectContextHandle = ASC->MakeEffectContext();
		GameplayEffectContextHandle.AddSourceObject(AvatarActor);
		
		const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, GameplayEffectContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

		const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(AuraCharacterClassInfo->SecondaryAttributes, Level, GameplayEffectContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

		const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(AuraCharacterClassInfo->VitalAttributes, Level, GameplayEffectContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
	}
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	if (const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		UAuraCharacterClassInfo* AuraCharacterClassInfo = AuraGameModeBase->CharacterClassInfo;
		check(AuraCharacterClassInfo);

		for (TSubclassOf<UGameplayAbility> AbilityClass : AuraCharacterClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			ASC->GiveAbility(AbilitySpec);
		}
	}
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

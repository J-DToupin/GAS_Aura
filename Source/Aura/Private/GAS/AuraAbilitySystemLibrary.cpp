// Copyright JDgame


#include "GAS/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Character/Player/AuraPlayerState.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
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
	if (UAuraCharacterClassInfo* AuraCharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		AActor* AvatarActor = ASC->GetAvatarActor();
		
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

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	
	if (UAuraCharacterClassInfo* AuraCharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		check(AuraCharacterClassInfo);

		for (TSubclassOf<UGameplayAbility> AbilityClass : AuraCharacterClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			ASC->GiveAbility(AbilitySpec);
		}
		const FCharacterClassDefaultInfo DefaultInfo = AuraCharacterClassInfo->GetClassDefaultInfo(CharacterClass);

		for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
		{
			if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor()));
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}

UAuraCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	if (const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return  AuraGameModeBase->CharacterClassInfo;
	}

	return nullptr;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (AuraGameplayEffectContext->GetDamageType().IsValid())
		{
			return *AuraGameplayEffectContext->GetDamageType();
		}
		
	}
	return FGameplayTag{};
}

void UAuraAbilitySystemLibrary::SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetCriticalHit(bInIsCriticalHit);
		
	}
}


void UAuraAbilitySystemLibrary::SetSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bSuccessfulDebuff)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetSuccessfulDebuff(bSuccessfulDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle,
	float NewDebuffDamage)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDebuffDamage(NewDebuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,
	float NewDebuffDuration)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDebuffDuration(NewDebuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,
	float NewDebuffFrequency)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDebuffFrequency(NewDebuffFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> NewDamageType = MakeShared<FGameplayTag>(InDamageType);
		AuraGameplayEffectContext->SetDamageType(NewDamageType);
	}
}


FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
	FGameplayEffectContextHandle EffectContextHandle{};
	if (Params.SourceAbilitySystemComponent == nullptr || Params.TargetAbilitySystemComponent == nullptr) return EffectContextHandle;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	const AActor* SourceAvatarActor = Params.SourceAbilitySystemComponent->GetAvatarActor();
	EffectContextHandle = Params.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	
	const FGameplayEffectSpecHandle DamageSpecHandle = Params.SourceAbilitySystemComponent->MakeOutgoingSpec(Params.DamageGameplayEffectClass, Params.AbilityLevel, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Params.DamageType, Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Damage, Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Frequency, Params.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Duration, Params.DebuffDuration);
	
	Params.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data);
	return EffectContextHandle;
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverLappingActors, const TArray<AActor*>& ActorToIgnore, float Radius,
                                                           const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorToIgnore);
	
	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), SphereParams);

		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverLappingActors.AddUnique(Overlap.GetActor());
			};
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(const AActor* FirstActor, const AActor* SecondActor)
{
	const bool FirstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool SecondIsPlayer = SecondActor->ActorHasTag(FName("Player"));

	if (!SecondActor->ActorHasTag("Player") &&  !SecondActor->ActorHasTag("Enemy"))
	{
		return true;
	}

	return FirstIsPlayer != SecondIsPlayer;
}

int32 UAuraAbilitySystemLibrary::GetXpOnEnemies(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const int32 Level)
{
	const FScalableFloat XpRewardCurve = GetCharacterClassInfo(WorldContextObject)->GetClassDefaultInfo(CharacterClass).XpReward;
	if (XpRewardCurve.IsValid())
	{
		return XpRewardCurve.GetValueAtLevel(Level);
	}

	return 0;
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

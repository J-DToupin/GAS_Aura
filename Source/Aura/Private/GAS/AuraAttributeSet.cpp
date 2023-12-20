// Copyright JDgame


#include "GAS/AuraAttributeSet.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Character/Player/AuraPlayerController.h"
#include "GameFramework/Character.h"
#include "GAS/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetResistanceFireAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetResistanceLightningAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetResistanceArcaneAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetResistancePhysicalAttribute);
	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceFire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceLightning, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceArcane, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistancePhysical, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

// function utiliser seulement pour clamping CLAMP seulement linformation et non la valeur
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, const float LocalIncomingDamage)
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		const bool bBlock = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		
		if (AAuraPlayerController* APC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
		{
			APC->ShowDamageNumber(LocalIncomingDamage, Props.TargetCharacter, bBlock, bCriticalHit);
			return;
		}

		if (AAuraPlayerController* APC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
		{
			APC->ShowDamageNumber(LocalIncomingDamage, Props.TargetCharacter, bBlock, bCriticalHit);
		}
				
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FEffectProperties Props = SetEffectProperties(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;

			if (bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
				SendXpEvent(Props);
			}
			else
			{
				// active une abilities par un tag
				FGameplayTagContainer TargetTagContainer;
				TargetTagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				if (Props.TargetASC)
				{
					Props.TargetASC->TryActivateAbilitiesByTag(TargetTagContainer);
				}
			}
			
			ShowFloatingText(Props, LocalIncomingDamage);
		}
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXpAttribute())
	{
		const float LocalIncomingXp = GetIncomingXp();
		SetIncomingXp(0.f);
		
		//Source Character is the owner, Since GA_ListenForEvents applies GE_EventBaseEffect
		ACharacter* OwnerCharacter = Props.SourceCharacter;
		if (OwnerCharacter && OwnerCharacter->Implements<UPlayerInterface>())
		{
			const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(OwnerCharacter);
			const int32 CurrentXp = IPlayerInterface::Execute_GetCurrentXp(OwnerCharacter);
			const int32 NextLevel = IPlayerInterface::Execute_GetLevelForXp(OwnerCharacter, CurrentXp + LocalIncomingXp);
			
			if (const int32 NumLevelUps = NextLevel - CurrentLevel)
			{
				const int32 AttributePointReward = IPlayerInterface::Execute_GetAttributePointsReward(OwnerCharacter, CurrentLevel);
				const int32 SpellPointReward =IPlayerInterface::Execute_GetSpellPointsReward(OwnerCharacter, CurrentLevel);

				IPlayerInterface::Execute_AddToPlayerLevel(OwnerCharacter, NumLevelUps);
				IPlayerInterface::Execute_AddToAttributePoint(OwnerCharacter, AttributePointReward);
				IPlayerInterface::Execute_AddToSpellPoint(OwnerCharacter, SpellPointReward);

				bTopOffHealth = true;
				bTopOffMana = true;
				
				IPlayerInterface::Execute_LevelUp(OwnerCharacter);
			}

			
			IPlayerInterface::Execute_AddToXp(OwnerCharacter, LocalIncomingXp);
		}
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}

	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
	
}

void UAuraAttributeSet::OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceFire, OldResistanceFire);
}

void UAuraAttributeSet::OnRep_ResistanceLightning(const FGameplayAttributeData& OldResistanceLightning) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceLightning, OldResistanceLightning);
}

void UAuraAttributeSet::OnRep_ResistanceArcane(const FGameplayAttributeData& OldResistanceArcane) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceArcane, OldResistanceArcane);
}

void UAuraAttributeSet::OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistancePhysical, OldResistancePhysical);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);

}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);

}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

FEffectProperties UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data)
{
	// Source = causer of the effect, target of the effect (owner of this AS)
	FEffectProperties Props;
	
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC))
	{
		Props.SourceAvatarActor = Props.SourceASC->GetAvatarActor();
		//const AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();

		Props.SourceController = Props.SourceAvatarActor->GetInstigatorController();

		if (Props.SourceController)
		{
			Props.SourceCharacter =  Props.SourceController->GetCharacter();
		}
		
	}

	if (IsValid(&Data.Target))
	{
		Props.TargetAvatarActor = Data.Target.GetAvatarActor();
		Props.TargetController = Props.TargetAvatarActor->GetInstigatorController();
		Props.TargetCharacter = Props.TargetController->GetCharacter();
		Props.TargetASC = &Data.Target;
	}

	return Props;
}

void UAuraAttributeSet::SendXpEvent(const FEffectProperties& Props)
{
	int32 XpReward{};
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		XpReward = UAuraAbilitySystemLibrary::GetXpOnEnemies(this,
						ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter),
						ICombatInterface::Execute_GetCharacterLevel(Props.TargetCharacter));
	}
	
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	FGameplayEventData Payload;
	Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXp;
	Payload.EventMagnitude = XpReward;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXp,Payload);
	
}

// Copyright JDgame


#include "GAS/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "GAS/AuraAbilitySystemLibrary.h"
#include "GAS/AuraAttributeSet.h"
#include "GAS/Data/AuraCharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	// 1. Declare
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceFire)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceLightning)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceArcane)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePhysical)

	
	AuraDamageStatics()
	{
		// 2. Define
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceFire, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceLightning, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceArcane, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistancePhysical, Target, false);
	}
};

static const  AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	// 3. Setup Attribute Capture
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceFireDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceLightningDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceArcaneDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistancePhysicalDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	for (auto Pair : GameplayTags.DamageTypeDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);

		if (TypeDamage >= 0)
		{

			// regarde si le debuff est appliquer selon la resistance de la target et les chance du debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluateParameters, TargetDebuffResistance);

			TargetDebuffResistance = FMath::Max(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) * 0.01f;

			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;

			// si le debuff est appliquer
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				UAuraAbilitySystemLibrary::SetSuccessfulDebuff(ContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);

				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor,DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().ResistanceFireDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().ResistanceLightningDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ResistanceArcaneDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().ResistancePhysicalDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	
	// 4. Setup Variable whit Capture Attribute Magnitude
	float TargetArmor{};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters,TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration{};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	float TargetBlockChance{};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	float SourceCriticalHitChance{};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float SourceCriticalHitDamage{};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	float TargetCriticalHitResistance{};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	// Debuff

	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters, TagsToCaptureDefs);

	//Get Damage Set by Caller Magnitude
	float Damage{};
	for (const auto& DamagePairTag : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = DamagePairTag.Key;
		const FGameplayTag ResistanceTag = DamagePairTag.Value;

		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s} in ExecCalc_Damage"), *ResistanceTag.ToString());

		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = TagsToCaptureDefs[ResistanceTag];

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp<float>(Resistance, 0.f, 100.f);

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		DamageTypeValue *= (100 - Resistance) * 0.01f;
		Damage += DamageTypeValue;
	}

	// Find the Coefficient by Level 
	const UAuraCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceActor);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"),FString{});

	float ArmorPenetrationCoefficient{};
	if (SourceActor->Implements<UCombatInterface>())
	{
		ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(ICombatInterface::Execute_GetCharacterLevel(SourceActor));
	}

	float EffectiveArmorCoefficient{};
	float EffectiveCriticalHitResistanceCoefficient{};
	if (TargetActor->Implements<UCombatInterface>())
	{
		const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"),FString{});
		EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(ICombatInterface::Execute_GetCharacterLevel(TargetActor));

		const FRealCurve* EffectiveCriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveCriticalHitResistance"),FString{});
		EffectiveCriticalHitResistanceCoefficient = EffectiveCriticalHitResistanceCurve->Eval(ICombatInterface::Execute_GetCharacterLevel(TargetActor));
	}
	

	
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	SourceCriticalHitChance *= (100 - TargetCriticalHitResistance * EffectiveCriticalHitResistanceCoefficient) * 0.01f;

	// Check if is a CriticalHit
	const bool bIsCriticalHit{SourceCriticalHitChance > FMath::RandRange(0, 100)};
	UAuraAbilitySystemLibrary::SetCriticalHit(EffectContextHandle, bIsCriticalHit);
	if (bIsCriticalHit)
	{
		Damage =  (Damage * 2) + SourceCriticalHitDamage;
		
	}
	
	// CapTure BlockChance on Target, and determine if there was a successful Block
	const bool bIsBlock{TargetBlockChance > FMath::RandRange(0, 100)};
	UAuraAbilitySystemLibrary::SetBlockedHit(EffectContextHandle, bIsBlock);
	if (bIsBlock)
	{
		Damage *= 0.5;
	}
	
	// ArmorPenetration ignore a percentage of the Target Armor
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) * 0.01f;
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) * 0.01f;

	
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

// Copyright JDgame


#include "..\..\..\Public\GAS\ExecCalc\ExecCalc_AttributeEvent.h"

#include "AuraGameplayTags.h"
#include "GAS/AuraAttributeSet.h"

void UExecCalc_AttributeEvent::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const float IncomingXp = Spec.GetSetByCallerMagnitude(GameplayTags.Attributes_Meta_IncomingXp, false);
	const float Strength = Spec.GetSetByCallerMagnitude(GameplayTags.Attributes_Primary_Strength, false);
	const float Intelligence = Spec.GetSetByCallerMagnitude(GameplayTags.Attributes_Primary_Intelligence, false);
	const float Resilience = Spec.GetSetByCallerMagnitude(GameplayTags.Attributes_Primary_Resilience, false);
	const float Vigor = Spec.GetSetByCallerMagnitude(GameplayTags.Attributes_Primary_Vigor, false);

	if (IncomingXp)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAuraAttributeSet::GetIncomingXpAttribute(), EGameplayModOp::Additive, IncomingXp));
	}

	if (Strength)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAuraAttributeSet::GetStrengthAttribute(), EGameplayModOp::Additive, Strength));
	}

	if (Intelligence)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAuraAttributeSet::GetIntelligenceAttribute(), EGameplayModOp::Additive, Intelligence));
	}
	
	if (Resilience)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAuraAttributeSet::GetResilienceAttribute(), EGameplayModOp::Additive, Resilience));
	}

	if (Vigor)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAuraAttributeSet::GetVigorAttribute(), EGameplayModOp::Additive, Vigor));
	}
	
}

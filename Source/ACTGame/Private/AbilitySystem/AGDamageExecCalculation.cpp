// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AGDamageExecCalculation.h"
#include "AbilitySystem/AGAttributeSet.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct AGDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Break);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Power);

	AGDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, Damage, Source, true);

		// Capture the Target's Armor. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, Break, Target, false);
	}
};

static const AGDamageStatics& DamageStatics()
{
	static AGDamageStatics DStatics;
	return DStatics;
}

UAGDamageExecCalculation::UAGDamageExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().BreakDef);
	RelevantAttributesToCapture.Add(DamageStatics().PowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefensePowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicDefenseDef);
}

void UAGDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	/**
	 * 伤害计算公式为 (0.5+AT-DF)*POW*RAND
	 * AT为攻击力，DF为防御力，POW为每个能力特有的数值
	 * RAND范围为0.95到1.05，Break状态下0.5替换为2.5
	 */
	float AttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);
	float DefensePower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefensePowerDef, EvaluationParameters, DefensePower);
	float Power = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().PowerDef, EvaluationParameters, Power);
	float MagicPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicPowerDef, EvaluationParameters, MagicPower);
	float MagicDefense = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicDefenseDef, EvaluationParameters, MagicDefense);

	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	//Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float MitigatedDamage = Damage + (0.5 + AttackPower - DefensePower) * Power * FMath::RandRange(0.95f, 1.05f);

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}

	// Broadcast damages to Target ASC
	UAGAbilitySystemComponent* TargetASC = Cast<UAGAbilitySystemComponent>(TargetAbilitySystemComponent);
	if (TargetASC)
	{
		UAGAbilitySystemComponent* SourceASC = Cast<UAGAbilitySystemComponent>(SourceAbilitySystemComponent);
		TargetASC->ReceiveDamage(SourceASC, MitigatedDamage);
	}
}
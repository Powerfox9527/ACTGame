// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AGDamageExecCalculation.h"
#include "AbilitySystem/AGAttributeSet.h"
#include "AGCharacterBase.h"
#include "ACTGame/Public/ACTGameCharacter.h"
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
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, MagicPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, Power, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, Break, Target, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGAttributeSet, MagicDefense, Target, false);

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

	WindMagicTag = FGameplayTag::RequestGameplayTag(FName("Magic.Wind"));
	FireMagicTag = FGameplayTag::RequestGameplayTag(FName("Magic.Fire"));
	IceMagicTag = FGameplayTag::RequestGameplayTag(FName("Magic.Ice"));
	ThunderMagicTag = FGameplayTag::RequestGameplayTag(FName("Magic.Thunder"));
}

void UAGDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	AAGCharacterBase* SourceCharacter = Cast<AAGCharacterBase>(SourceActor);
	AAGCharacterBase* TargetCharacter = Cast<AAGCharacterBase>(TargetActor);
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	FGameplayTagContainer container;
	Spec.GetAllAssetTags(container);
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
	FText text;
	
	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	//Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float MitigatedDamage = 0.0f;

	float Break = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BreakDef, EvaluationParameters, Break);
	//ConstantPower用于计算Break值满或属性相克的情况
	//火与冰互克，风与雷互克
	float ConstantPower = 0.5f;

	if (TargetCharacter->IsBreaked == true)
	{
		ConstantPower += 2.0f;
	}

	Break = 0.0f;
	if (container.Num() > 0)
	{
		if(container.HasTag(WindMagicTag))
		{
			if (TargetCharacter->WeakPoints.HasAny(container))
			{
				ConstantPower += 1;
				Break +=  0.25f * FMath::FRandRange(0.2f, 0.7f) / (MagicPower - MagicDefense);
				text = FText::FromString("WeakPoint");
			}
		}
		MitigatedDamage = Damage + (ConstantPower + MagicPower - MagicDefense) * Power * FMath::FRandRange(0.9f, 1.1f);
	}
	else
	{
		MitigatedDamage = Damage + (ConstantPower + AttackPower - DefensePower) * Power * FMath::FRandRange(0.9f, 1.1f);
	}
	Break += 0.06f * Power;
	if (TargetCharacter->IsBreaked)
	{
		Break = 0.0f;
	}
	MitigatedDamage = FMath::RoundToInt(MitigatedDamage);
	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().BreakProperty, EGameplayModOp::Additive, Break));
	}

	// Broadcast damages to Target ASC
	UAGAbilitySystemComponent* TargetASC = Cast<UAGAbilitySystemComponent>(TargetAbilitySystemComponent);
	if (TargetASC)
	{
		UAGAbilitySystemComponent* SourceASC = Cast<UAGAbilitySystemComponent>(SourceAbilitySystemComponent);
		TargetASC->ReceiveDamage(SourceASC, MitigatedDamage);
	}

	//show damage Number
	TargetCharacter->ShowDamageNumber(MitigatedDamage, text);
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AGDamageExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UAGDamageExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UAGDamageExecCalculation();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	FGameplayTag WindMagicTag;
	FGameplayTag FireMagicTag;
	FGameplayTag IceMagicTag;
	FGameplayTag ThunderMagicTag;
};

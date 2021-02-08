// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "ACTGame/Public/AGCharacterBase.h"
#include "BTTask_ActivateAbilityByTags.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UBTTask_ActivateAbilityByTags : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ActivateAbility(AActor* controlledPawn, FGameplayTag tag);
};

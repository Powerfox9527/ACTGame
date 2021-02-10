// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "Animation/AnimMontage.h"
#include "AGGA_Roll.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UAGGA_Roll : public UAGGameplayAbility
{
	GENERATED_BODY()
public:
	
	UAGGA_Roll();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
		void OnFinished();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RollLength = 2000;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RollTime = 0.0f;

public:
	
};

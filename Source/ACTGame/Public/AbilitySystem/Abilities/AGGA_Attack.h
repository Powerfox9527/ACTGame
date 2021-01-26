// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AGGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UAGGA_Attack : public UAGGameplayAbility
{
	GENERATED_BODY()

	UAGGA_Attack();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* MontageToPlay;
};

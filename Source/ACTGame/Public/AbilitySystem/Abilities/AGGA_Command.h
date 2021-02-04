// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AGGA_Command.generated.h"

class AACTGameCharacter;
/**
 * 
 */
UCLASS()
class ACTGAME_API UAGGA_Command : public UAGGameplayAbility
{
	GENERATED_BODY()

	UAGGA_Command();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};

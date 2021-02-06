// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AGProjectile.h"
#include "AGGA_Magic.generated.h"

class AAGProjectile;
/**
 * 
 */
UCLASS()
class ACTGAME_API UAGGA_Magic : public UAGGameplayAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	public:
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AAGProjectile> ProjectileClass;
};

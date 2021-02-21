// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AGProjectile.h"
#include "ACTGame/Public/ACTGameCharacter.h"
#include "AGGA_Accumulate.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UAGGA_Accumulate : public UAGGameplayAbility
{
	GENERATED_BODY()
public:

	UAGGA_Accumulate();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<AAGProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Damage;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(BlueprintCallable)
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};

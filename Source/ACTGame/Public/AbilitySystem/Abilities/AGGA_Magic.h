// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AGProjectile.h"
#include "AbilitySystem/AGGameplayEffect.h"
#include "ACTGame/Public/AGCharacterBase.h"
#include "ACTGame/Public/WeaponActor.h"
#include "AGGA_Magic.generated.h"

class AAGProjectile;
class AAGCharacterBase;
class AWeaponActor;
/**
 * 
 */
UCLASS()
class ACTGAME_API UAGGA_Magic : public UAGGameplayAbility
{
	GENERATED_BODY()

	UAGGA_Magic();
	
	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AAGProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Damage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool AttackMode = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 ProjectileCount = 1;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(BlueprintCallable)
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};

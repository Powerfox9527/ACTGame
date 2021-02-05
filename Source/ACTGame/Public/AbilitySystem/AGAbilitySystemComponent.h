// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AGAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReceivedDamageDelegate, UAGAbilitySystemComponent*, SourceASC, float, MitigatedDamage);

class UAGGameplayAbility;
/**
 * 
 */
UCLASS()
class ACTGAME_API UAGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

// 	UFUNCTION(BlueprintCallable)
// 	TArray<UAGGameplayAbility*> GetAbilitiesByTagContainer(const FGameplayTagContainer &tagContainer);
public:
	FReceivedDamageDelegate ReceivedDamage;
	// Called from AGDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UAGAbilitySystemComponent* SourceASC, float MitigatedDamage);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AGGA_Command.h"

void UAGGA_Command::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GetWorld()->GetWorldSettings()->SetTimeDilation(0.01);
}

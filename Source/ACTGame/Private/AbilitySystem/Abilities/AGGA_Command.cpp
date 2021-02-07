// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AGGA_Command.h"
#include "ACTGameCharacter.h"

UAGGA_Command::UAGGA_Command()
{
	FGameplayTag AbilityInputTag = FGameplayTag::RequestGameplayTag(FName("AbilityInputID.Command"));
	AbilityTags.AddTag(AbilityInputTag);
	ActivationOwnedTags.AddTag(AbilityInputTag);
}

void UAGGA_Command::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AWorldSettings *settings  = GetWorld()->GetWorldSettings();
	bool flag = CommitAbility(Handle, ActorInfo, ActivationInfo);
	if (flag == false)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	settings->SetTimeDilation(0.01);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

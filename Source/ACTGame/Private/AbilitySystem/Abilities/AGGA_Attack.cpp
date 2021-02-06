// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AGGA_Attack.h"
#include "ACTGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AbilityTask/AGAT_PlayMontageAndWaitForEvent.h"

UAGGA_Attack::UAGGA_Attack()
{
	FGameplayTag AbilityInputTag = FGameplayTag::RequestGameplayTag(FName("AbilityInputID.Attack"));
	AbilityTags.AddTag(AbilityInputTag);
}

void UAGGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bool flag = CommitAbility(Handle, ActorInfo, ActivationInfo);
	AACTGameCharacter* OwningActor = Cast<AACTGameCharacter>(GetOwningActorFromActorInfo());
	if (flag == false || OwningActor == nullptr || (OwningActor->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() && OwningActor->ComboSectionName.IsNone()))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	UAGAT_PlayMontageAndWaitForEvent* Task = UAGAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, FName(TEXT("Attack")), MontageToPlay, FGameplayTagContainer(), 1.0f, OwningActor->ComboSectionName, false, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UAGGA_Attack::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UAGGA_Attack::OnCompleted);
	Task->OnCancelled.AddDynamic(this, &UAGGA_Attack::OnCompleted);
	Task->OnBlendOut.AddDynamic(this, &UAGGA_Attack::OnCompleted);
	Task->ReadyForActivation();
	OwningActor->GetCharacterMovement()->SetActive(false);
}

void UAGGA_Attack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AACTGameCharacter* OwningActor = Cast<AACTGameCharacter>(GetOwningActorFromActorInfo());
	OwningActor->GetCharacterMovement()->SetActive(true);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAGGA_Attack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
}


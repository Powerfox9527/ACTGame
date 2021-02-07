// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AGGA_Roll.h"
#include "AbilitySystem/AbilityTask/AGAT_PlayMontageAndWaitForEvent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "ACTGameCharacter.h"

UAGGA_Roll::UAGGA_Roll()
{
	FGameplayTag AbilityInputTag = FGameplayTag::RequestGameplayTag(FName("AbilityInputID.Roll"));
	AbilityTags.AddTag(AbilityInputTag);
	ActivationOwnedTags.AddTag(AbilityInputTag);
}

void UAGGA_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bool flag = CommitAbility(Handle, ActorInfo, ActivationInfo);
	AACTGameCharacter* OwningActor = Cast<AACTGameCharacter>(GetOwningActorFromActorInfo());
	if (flag == false || OwningActor == nullptr || OwningActor->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	UAGAT_PlayMontageAndWaitForEvent* Task = UAGAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	Task->OnCompleted.AddDynamic(this, &UAGGA_Roll::OnCompleted);
	Task->ReadyForActivation();
	UAbilityTask_ApplyRootMotionConstantForce* TaskRootMotion = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(this, "Dash", OwningActor->GetActorForwardVector(), 1000, 0.3, false, nullptr, ERootMotionFinishVelocityMode::SetVelocity, OwningActor->GetVelocity(), 0, false);
	TaskRootMotion->OnFinish.AddDynamic(this, &UAGGA_Roll::OnFinished);
	TaskRootMotion->ReadyForActivation();
}

void UAGGA_Roll::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	AACTGameCharacter* OwningActor = Cast<AACTGameCharacter>(GetOwningActorFromActorInfo());
}

void UAGGA_Roll::OnFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	AACTGameCharacter* OwningActor = Cast<AACTGameCharacter>(GetOwningActorFromActorInfo());
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AGGA_Accumulate.h"
#include "AbilitySystem/AbilityTask/AGAT_PlayMontageAndWaitForEvent.h"

UAGGA_Accumulate::UAGGA_Accumulate()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	Range = 1000.0f;
	Damage = 12.0f;
}

void UAGGA_Accumulate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
	AACTGameCharacter* OwningActor = Cast<AACTGameCharacter>(GetOwningActorFromActorInfo());
	if (OwningActor == nullptr)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	if (OwningActor->AbilityTarget != nullptr)
	{
		OwningActor->RotateToActor(OwningActor->AbilityTarget);
	}
	OwningActor->GetMesh()->bPauseAnims = false;
	// Play fire montage and wait for event telling us to spawn the projectile
	UAGAT_PlayMontageAndWaitForEvent* Task = UAGAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, FName("Accumulate"), MontageToPlay, FGameplayTagContainer(), 1.0f, FName("StyleReleased"), false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UAGGA_Accumulate::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UAGGA_Accumulate::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UAGGA_Accumulate::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UAGGA_Accumulate::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UAGGA_Accumulate::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UAGGA_Accumulate::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UAGGA_Accumulate::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UAGGA_Accumulate::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
	{
		AAGCharacterBase* Hero = Cast<AAGCharacterBase>(GetAvatarActorFromActorInfo());
		if (!Hero)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());

		// Pass the damage to the Damage Execution Calculation through a SetByCaller value on the GameplayEffectSpec
		DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);
		FTransform ProjectileTransform;
		if (Hero)
		{
			ProjectileTransform = Hero->GetProjectileTransform();
		}
		else
		{
			ProjectileTransform = Hero->GetActorTransform();
		}
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AAGProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAGProjectile>(ProjectileClass, ProjectileTransform, GetOwningActorFromActorInfo(),
			Hero, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (Projectile == nullptr)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}
		Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
		Projectile->Range = Range;
		Projectile->OwningCharacter = Cast<AAGCharacterBase>(this->GetOwningActorFromActorInfo());
		Projectile->FinishSpawning(ProjectileTransform);
	}

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.PauseAnim")))
	{
		AAGCharacterBase* Hero = Cast<AAGCharacterBase>(GetOwningActorFromActorInfo());
		Hero->GetMesh()->bPauseAnims = true;
		Hero->EnhancePower(1.5f);
	}

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.ContinueAnim")))
	{
		AAGCharacterBase* Hero = Cast<AAGCharacterBase>(GetOwningActorFromActorInfo());
		Hero->GetMesh()->bPauseAnims = false;
	}
}

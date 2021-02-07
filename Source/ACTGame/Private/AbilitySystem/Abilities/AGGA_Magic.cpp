// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AGGA_Magic.h"
#include "AbilitySystem/AbilityTask/AGAT_PlayMontageAndWaitForEvent.h"

UAGGA_Magic::UAGGA_Magic()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

// 	FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Ability1"));
// 	AbilityTags.AddTag(Ability1Tag);
// 	ActivationOwnedTags.AddTag(Ability1Tag);
// 
// 	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));

	FGameplayTag AbilityInputTag = FGameplayTag::RequestGameplayTag(FName("Command.Magic"));
	AbilityTags.AddTag(AbilityInputTag);
	ActivationOwnedTags.AddTag(AbilityInputTag);

	Range = 1000.0f;
	Damage = 12.0f;
}

void UAGGA_Magic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	// Play fire montage and wait for event telling us to spawn the projectile
	UAGAT_PlayMontageAndWaitForEvent* Task = UAGAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UAGGA_Magic::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UAGGA_Magic::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UAGGA_Magic::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UAGGA_Magic::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UAGGA_Magic::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UAGGA_Magic::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UAGGA_Magic::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAGGA_Magic::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
}

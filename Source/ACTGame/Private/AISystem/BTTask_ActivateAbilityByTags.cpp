// Fill out your copyright notice in the Description page of Project Settings.


#include "AISystem/BTTask_ActivateAbilityByTags.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"

float UBTTask_ActivateAbilityByTags::ActivateAbility(AActor* controlledPawn, FGameplayTag tag)
{
	AAGCharacterBase* character = Cast<AAGCharacterBase>(controlledPawn);
	if (character == nullptr)
		return 0.0f;
	TArray<TSubclassOf<UAGGameplayAbility>> activateAbilities;
	TArray<TSubclassOf<UAGGameplayAbility>> abilities = character->CharacterAbilities;
	for (TSubclassOf<UAGGameplayAbility> ability : abilities)
	{
		UAGGameplayAbility* abilityInstance = ability.GetDefaultObject();
		if (abilityInstance->AbilityTags.HasTag(tag))
		{
			activateAbilities.Add(ability);
		}
	}
	if (activateAbilities.Num() == 0)
	{
		return 0.0f;
	}
	int32 index = FMath::RandRange(0, activateAbilities.Num()-1);
	TSubclassOf<UAGGameplayAbility> activateClass = activateAbilities[index];
	character->GetAbilitySystemComponent()->TryActivateAbilityByClass(activateClass);
	UAnimMontage* montage = activateClass.GetDefaultObject()->MontageToPlay;
	if(montage == nullptr)
		return 0.0f;
	float montageTime = montage->GetSectionLength(0);
	return montageTime;
	//character->GetAbilitySystemComponent()->TryActivateAbilitiesByClass(activateClass);
}

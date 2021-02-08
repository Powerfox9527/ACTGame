// Fill out your copyright notice in the Description page of Project Settings.


#include "AISystem/BTTask_ActivateAbilityByTags.h"

void UBTTask_ActivateAbilityByTags::ActivateAbility(AActor* controlledPawn, FGameplayTag tag)
{
	AAGCharacterBase* character = Cast<AAGCharacterBase>(controlledPawn);
	if (character == nullptr)
		return;
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
		return;
	}
	int32 index = FMath::RandRange(0, activateAbilities.Num()-1);
	TSubclassOf<UAGGameplayAbility> activateClass = activateAbilities[index];
	character->GetAbilitySystemComponent()->TryActivateAbilityByClass(activateClass);
	//character->GetAbilitySystemComponent()->TryActivateAbilitiesByClass(activateClass);
}

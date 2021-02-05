// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AGAbilitySystemComponent.h"

// TArray<UAGGameplayAbility*> UAGAbilitySystemComponent::GetAbilitiesByTagContainer(const FGameplayTagContainer& tagContainer)
// {
// 	TArray<FGameplayAbilitySpec> ActivatableAbilities = GetActivatableAbilities();
// 	TArray<UAGGameplayAbility*> returnAbilities;
// 	for (FGameplayAbilitySpec abilitySpec : ActivatableAbilities)
// 	{
// 		bool canTakeAbility = true;
// 		for (FGameplayTag tag : tagContainer)
// 		{
// 			if (!abilitySpec.Ability->AbilityTags.HasTag(tag))
// 			{
// 				canTakeAbility = false;
// 				break;
// 			}
// 		}
// 		if (canTakeAbility)
// 		{
// 			UAGGameplayAbility *ability = Cast<UAGGameplayAbility>(abilitySpec.Ability);
// 			if (ability != nullptr)
// 			{
// 				returnAbilities.Add(ability);
// 			}
// 		}
// 	}
// 
// 	return returnAbilities;
// }

void UAGAbilitySystemComponent::ReceiveDamage(UAGAbilitySystemComponent* SourceASC, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, MitigatedDamage);
}

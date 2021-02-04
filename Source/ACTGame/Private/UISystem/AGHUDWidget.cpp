// Fill out your copyright notice in the Description page of Project Settings.


#include "UISystem/AGHUDWidget.h"
#include "Kismet/GameplayStatics.h"

void UAGHUDWidget::SetCommandViewSelection(bool IsUp)
{
	if(AbilityList == nullptr || AbilityList->GetNumItems() == 0)
		return;
	UObject* selectedItem = AbilityList->GetSelectedItem();
	int32 nowIndex = AbilityList->GetIndexForItem(selectedItem);
	int32 listSize = AbilityList->GetNumItems();
	int32 selectIndex = nowIndex;
	if (nowIndex == 0 && IsUp)
	{
		selectIndex = listSize - 1;
	}
	else
	{
		selectIndex = (nowIndex + (IsUp ? -1 : 1)) % listSize;
	}
	AbilityList->SetSelectedIndex(selectIndex);
}

void UAGHUDWidget::SetCommandViewByTag(FGameplayTag tag)
{
	if (AbilityList == nullptr)
		return;
	AbilityList->ClearListItems();
	if (tag == FGameplayTag::EmptyTag)
	{
		AbilityNames.Empty();
		AbilityNames.Add(FText::FromString("Ability"));
		AbilityNames.Add(FText::FromString("Magic"));
		AbilityNames.Add(FText::FromString("Item"));
	}
	else
	{
		AbilityNames.Empty();
		AACTGameCharacter *character =  Cast<AACTGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		TArray<TSubclassOf<UAGGameplayAbility>> abilities =  character->CharacterAbilities;
		for (TSubclassOf<UAGGameplayAbility> ability : abilities)
		{
			UAGGameplayAbility *abilityInstance = ability.GetDefaultObject();
			if (abilityInstance->AbilityTags.HasTag(tag))
			{
				if (abilityInstance->AbilityName.IsEmpty())
				{
					AbilityNames.Add(FText::FromName(abilityInstance->GetFName()));
				}
				else
				{
					AbilityNames.Add(abilityInstance->AbilityName);
				}
			}
		}
	}
	for (FText name : AbilityNames)
	{
		UAbilityEntryObj* Obj = NewObject<UAbilityEntryObj>();
		Obj->AbilityName = name;
		AbilityList->AddItem(Obj);
	}
	AbilityList->SetSelectedIndex(0);
}

bool UAGHUDWidget::ConfirmCommand()
{
	if (AbilityList == nullptr || AbilityList->GetNumItems() == 0 || !AbilityList->IsVisible())
		return false;
	UObject *Item = AbilityList->GetSelectedItem();
	int32 index = AbilityList->GetIndexForItem(Item);
	FText name = AbilityNames[index];
	if (name.ToString() == "Ability")
	{
		FGameplayTag tag = FGameplayTag::RequestGameplayTag(FName("Command.Ability"));
		SetCommandViewByTag(tag);
	}
	else if (name.ToString() == "Magic")
	{
		FGameplayTag tag = FGameplayTag::RequestGameplayTag(FName("Command.Magic"));
		SetCommandViewByTag(tag);
	}
	else if (name.ToString() == "Item")
	{
		FGameplayTag tag = FGameplayTag::RequestGameplayTag(FName("Command.Item"));
		SetCommandViewByTag(tag);
	}
	else
	{
		//AACTGameCharacter* character = Cast<AACTGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		AACTGameGameMode* GameMode = Cast<AACTGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		AACTGameCharacter* character = Cast<AACTGameCharacter>(GameMode->MainCharacter);
		TArray<TSubclassOf<UAGGameplayAbility>> abilities = character->CharacterAbilities;
		for (TSubclassOf<UAGGameplayAbility> ability : abilities)
		{
			UAGGameplayAbility* abilityInstance = ability.GetDefaultObject();
			if (abilityInstance->GetFName().ToString() == name.ToString() || abilityInstance->AbilityName.ToString() == name.ToString())
			{
				character->GetAbilitySystemComponent()->TryActivateAbilityByClass(ability);
				return true;
			}
		}
	}
	return false;
}

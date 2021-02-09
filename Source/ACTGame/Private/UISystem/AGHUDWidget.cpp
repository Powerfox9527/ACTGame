// Fill out your copyright notice in the Description page of Project Settings.


#include "UISystem/AGHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ACTGame/Public/AGEnemyBase.h"

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
	AbilityNames.Empty();
	AbilityAvailables.Empty();
	//LastTags.Add(tag);
	if (tag == FGameplayTag::RequestGameplayTag(FName("Command.Main")))
	{
		AbilityNames.Add(FText::FromString("Ability"));
		AbilityNames.Add(FText::FromString("Magic"));
		AbilityNames.Add(FText::FromString("Item"));
		for(int i = 0; i < 3; ++i)
			AbilityAvailables.Add(true);
	}
	else if (tag == FGameplayTag::RequestGameplayTag(FName("Command.Enemy")))
	{
		AAGEnemyBase* EnemyGetClass = NewObject<AAGEnemyBase>();
		TArray<AActor*> Enemys; 
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyGetClass->StaticClass(), Enemys);
		for (AActor* enemyActor : Enemys)
		{
			AAGEnemyBase* enemy = Cast<AAGEnemyBase>(enemyActor);
			if (enemy != nullptr || !enemy->IsAlive())
			{
				if (enemy->SelfName.IsEmpty())
				{
					AbilityNames.Add(FText::FromName(enemy->GetFName()));
				}
				else
				{
					AbilityNames.Add(enemy->SelfName);
				}
				AbilityAvailables.Add(true);
			}
		}
	}
	else if (tag == FGameplayTag::RequestGameplayTag(FName("Command.Friend")))
	{
		AACTGameCharacter* FriendGetClass = NewObject<AACTGameCharacter>();
		TArray<AActor*> Friends;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), FriendGetClass->StaticClass(), Friends);
		for (AActor* friendActor : Friends)
		{
			AACTGameCharacter* friendCharacter = Cast<AACTGameCharacter>(friendActor);
			if (friendCharacter != nullptr || !friendCharacter->IsAlive())
			{
				if (friendCharacter->SelfName.IsEmpty())
				{
					AbilityNames.Add(FText::FromName(friendCharacter->GetFName()));
				}
				else
				{
					AbilityNames.Add(friendCharacter->SelfName);
				}
				AbilityAvailables.Add(true);
			}
		}
	}
	else
	{
		AACTGameGameMode* GameMode = Cast<AACTGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		AACTGameCharacter* character = Cast<AACTGameCharacter>(GameMode->MainCharacter);
		TArray<TSubclassOf<UAGGameplayAbility>> abilities =  character->CharacterAbilities;
		for (TSubclassOf<UAGGameplayAbility> ability : abilities)
		{
			UAGGameplayAbility *abilityInstance = NewObject<UAGGameplayAbility>(this, ability);
			bool available = abilityInstance->IsAvailable();

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
				AbilityAvailables.Add(available);
			}
		}
	}
	int32 AbilityNum = AbilityNames.Num();
	for (int32 i = 0; i < AbilityNum; ++i)
	{
		FText name = AbilityNames[i];
		bool available = AbilityAvailables[i];
		UAbilityEntryObj* Obj = NewObject<UAbilityEntryObj>();
		Obj->AbilityName = name;
		Obj->Available = available;
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
	if(!AbilityAvailables[index])
		return false;
	if (name.ToString() == "Ability")
	{
		SelectTag = FGameplayTag::RequestGameplayTag(FName("Command.Ability"));
		SetCommandViewByTag(SelectTag);
		LastTags.Push(FGameplayTag::RequestGameplayTag(FName("Command.Main")));
	}
	else if (name.ToString() == "Magic")
	{
		SelectTag = FGameplayTag::RequestGameplayTag(FName("Command.Magic"));
		SetCommandViewByTag(SelectTag);
		LastTags.Push(FGameplayTag::RequestGameplayTag(FName("Command.Main")));
	}
	else if (name.ToString() == "Item")
	{
		SelectTag = FGameplayTag::RequestGameplayTag(FName("Command.Item"));
		SetCommandViewByTag(SelectTag);
		LastTags.Push(FGameplayTag::RequestGameplayTag(FName("Command.Main")));
	}
	//选定能力种类后选定施展对象,这里记住一显示CommandView就要清空SelectTag
	else if (SelectAbilityName.IsEmpty())
	{
		if (SelectTag == FGameplayTag::RequestGameplayTag(FName("Command.Item")))
		{
			LastTags.Push(SelectTag);
			SelectTag = FGameplayTag::RequestGameplayTag(FName("Command.Friend"));
			SetCommandViewByTag(SelectTag);
			SelectAbilityName = name;
		}
		else if (SelectTag == FGameplayTag::RequestGameplayTag(FName("Command.Ability"))
			|| SelectTag == FGameplayTag::RequestGameplayTag(FName("Command.Magic")))
		{
			LastTags.Push(SelectTag);
			SelectTag = FGameplayTag::RequestGameplayTag(FName("Command.Enemy"));
			SetCommandViewByTag(SelectTag);
			SelectAbilityName = name;
		}
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
			if (abilityInstance->GetFName().ToString() == SelectAbilityName.ToString() || abilityInstance->AbilityName.ToString() == SelectAbilityName.ToString())
			{
				AAGCharacterBase* CharacterGetClass = NewObject<AAGCharacterBase>();
				TArray<AActor*> Characters;
				AAGCharacterBase* SelectCharacter = nullptr;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), CharacterGetClass->StaticClass(), Characters);
				for (AActor* characterActor : Characters)
				{
					AAGCharacterBase* TempCharacter = Cast<AAGCharacterBase>(characterActor);
					if (TempCharacter)
					{
						if (TempCharacter->SelfName.ToString() == name.ToString() || TempCharacter->GetFName().ToString() == name.ToString())
						{
							SelectCharacter = TempCharacter;
						}
					}
				}
				//character->RotateToActor(SelectCharacter);
				character->SetAbilityTarget(SelectCharacter);
				character->GetAbilitySystemComponent()->TryActivateAbilityByClass(ability);
				AbilityList->ClearSelection();
				return true;
			}
		}
	}
	return false;
}

bool UAGHUDWidget::ReturnToLastCommandView()
{
	// 先这样定返回到最上级的规则吧
	if (AbilityList == nullptr || LastTags.Num() == 0 || LastTags.Last() == FGameplayTag::EmptyTag)
	{
		return true;
	}
	else
	{
		SetCommandViewByTag(LastTags.Pop());
		return false;
	}
}

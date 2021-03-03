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
	else if (SelectAbilityName.IsEmpty())
	{
// 		if (SelectTag == FGameplayTag::RequestGameplayTag(FName("Command.Item"))
// 		|| SelectTag == FGameplayTag::RequestGameplayTag(FName("Command.WhiteMagic")))
// 		{
// 			LastTags.Push(SelectTag);
// 			SelectTag = FGameplayTag::RequestGameplayTag(FName("Command.Friend"));
// 			SetCommandViewByTag(SelectTag);
// 			SelectAbilityName = name;
// 		}
// 		else if (SelectTag == FGameplayTag::RequestGameplayTag(FName("Command.Ability"))
// 			|| SelectTag == FGameplayTag::RequestGameplayTag(FName("Command.Magic")))
// 		{
// 			LastTags.Push(SelectTag);
// 			SelectTag = FGameplayTag::RequestGameplayTag(FName("Command.Enemy"));
// 			SetCommandViewByTag(SelectTag);
// 			SelectAbilityName = name;
// 		}
		TSubclassOf<UAGGameplayAbility> AbilityClass;
		if (FindAbilityByName(name.ToString(), AbilityClass))
		{
			UAGGameplayAbility* ability = AbilityClass.GetDefaultObject();
			if (ability != nullptr)
			{
				LastTags.Push(SelectTag);
				SelectTag = ability->CommandRequestTag;
				SetCommandViewByTag(SelectTag);
				SelectAbilityName = name;
			}
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

void UAGHUDWidget::SwitchCharacter(bool IsLeft)
{
	if(!bCanSwitchCharacter)
		return;
	AACTGameGameMode* GameMode = Cast<AACTGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AACTGameCharacter* PreCharacter = Cast<AACTGameCharacter>(GameMode->MainCharacter);
	TArray<AActor*> characterActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AACTGameCharacter::StaticClass(), characterActors);
	if(characterActors.Num() == 0 || characterActors.Num() == 1)
		return;
	TArray<AACTGameCharacter*> characters;
	for (int32 i = 0; i < characterActors.Num(); ++i)
	{
		AACTGameCharacter* tempCharacter = Cast<AACTGameCharacter>(characterActors[i]);
		characters.Add(tempCharacter);
	}
	int32 preIndex = characters.Find(PreCharacter);
	int32 index = preIndex;
	if (IsLeft)
	{
		if(index == 0)
			index = characters.Num() - 1;
		else
			index--;
	}
	else

	{
		index = (index + 1) % characters.Num();
	}
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->SetViewTargetWithBlend(characters[index], 1.5f, EViewTargetBlendFunction::VTBlend_EaseInOut, 10.0f);
	GameMode->MainCharacter = characters[index];
	FTimerHandle UnusedHandle;
	PreCharacter->GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &UAGHUDWidget::SwitchCharacter_CameraBlendComplete, 1.5f, false);
	bCanSwitchCharacter = false;
}
void UAGHUDWidget::SwitchCharacter_CameraBlendComplete()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AACTGameGameMode* GameMode = Cast<AACTGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	controller->Possess(GameMode->MainCharacter);
	bCanSwitchCharacter = true;
}

bool UAGHUDWidget::FindAbilityByName(FString AbilityName, TSubclassOf<UAGGameplayAbility>& ability)
{
	//AACTGameCharacter* character = Cast<AACTGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AACTGameGameMode* GameMode = Cast<AACTGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AACTGameCharacter* character = Cast<AACTGameCharacter>(GameMode->MainCharacter);
	TArray<TSubclassOf<UAGGameplayAbility>> abilities = character->CharacterAbilities;
	for (TSubclassOf<UAGGameplayAbility> tempAbility : abilities)
	{
		UAGGameplayAbility* abilityInstance = tempAbility.GetDefaultObject();
		if (abilityInstance->GetFName().ToString() == AbilityName || abilityInstance->AbilityName.ToString() == AbilityName)
		{
			ability = tempAbility;
			return true;
		}
	}
	return false;
}

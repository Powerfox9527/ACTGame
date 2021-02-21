// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/ListView.h"
#include "ACTGame/Public/ACTGameCharacter.h"
#include "ACTGame/ACTGameGameMode.h"
#include "AGHUDWidget.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActivateAbilityDelegate, FText, AbilityName);

class UListView;
class ACTGameCharacter;
class AACTGameGameMode;
/**
 * 
 */
UCLASS(BlueprintType)
class ACTGAME_API UAbilityEntryObj : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText AbilityName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Available = true;
};

UCLASS()
class ACTGAME_API UAGHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void SetCommandViewSelection(bool IsUp);

	UFUNCTION(BlueprintCallable)
	void SetCommandViewByTag(FGameplayTag tag);

	// 以下两个函数，true代表要退出命令界面
	UFUNCTION(BlueprintCallable)
	bool ConfirmCommand();

	UFUNCTION(BlueprintCallable)
	bool ReturnToLastCommandView();

	UFUNCTION(BlueprintCallable)
	void SwitchCharacter(bool IsLeft);

	void SwitchCharacter_CameraBlendComplete();

	UFUNCTION(BlueprintCallable)
	 bool FindAbilityByName(FString AbilityName, TSubclassOf<UAGGameplayAbility>& ability);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UListView* AbilityList;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FGameplayTag> LastTags;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FText> AbilityNames;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<bool> AbilityAvailables;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText SelectAbilityName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag SelectTag;

	bool bCanSwitchCharacter = true;

// 	UPROPERTY(BlueprintAssignable)
// 	FActivateAbilityDelegate OnActivate;
};

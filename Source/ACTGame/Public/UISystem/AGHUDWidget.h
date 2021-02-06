// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/ListView.h"
#include "AbilitySystemComponent.h"
#include "ACTGameCharacter.h"
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
};

UCLASS()
class ACTGAME_API UAGHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void SetCommandViewSelection(bool IsUp);

	UFUNCTION(BlueprintCallable)
	void SetCommandViewByTag(FGameplayTag tag);

	UFUNCTION(BlueprintCallable)
	bool ConfirmCommand();

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UListView* AbilityList;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FText> AbilityNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText SelectAbilityName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag SelectTag;

// 	UPROPERTY(BlueprintAssignable)
// 	FActivateAbilityDelegate OnActivate;
};

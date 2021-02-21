// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UAGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText AbilityName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* MontageToPlay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FGameplayTag CommandRequestTag = FGameplayTag::RequestGameplayTag(FName("Command.Enemy"));

	//用于判断是否满足ATB或者法术条件
	UFUNCTION(BlueprintNativeEvent)
	bool IsAvailable();

	bool IsAvailable_Implementation();
};

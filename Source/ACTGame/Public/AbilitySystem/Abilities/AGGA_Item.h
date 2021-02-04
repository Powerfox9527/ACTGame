// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AGGA_Item.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UAGGA_Item : public UAGGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* MontageToPlay;

	UPROPERTY(BlueprintReadWrite)
		int32 ItemCount;
};

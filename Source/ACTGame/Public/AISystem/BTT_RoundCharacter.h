// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_RoundCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UBTT_RoundCharacter : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FVector GetNewRoundLocation(FVector OriginLocation, FVector NowLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void AroundCharacter();
};

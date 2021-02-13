// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AGBlueprintFunctionLibrary.generated.h"



/**
 * 
 */
UCLASS()
class ACTGAME_API UAGBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void DebugHelper(float value)
	{
		FString TheFloatStr = FString::SanitizeFloat(value);
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);
	}
};

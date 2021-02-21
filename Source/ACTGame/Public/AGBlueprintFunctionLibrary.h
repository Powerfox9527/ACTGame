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
		//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);
	}

	UFUNCTION(BlueprintCallable)
	static AActor* GetClosetActorOfClass(AActor* Origin, TSubclassOf<AActor> ActorClass);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	static bool IsBothPlayer(ACharacter* CharacterA, ACharacter* CharacterB);
};

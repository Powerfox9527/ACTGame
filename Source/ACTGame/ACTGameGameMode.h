// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "ACTGameGameMode.generated.h"

UCLASS()
class AACTGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AACTGameGameMode();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACharacter* MainCharacter;

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchCharacter();
};




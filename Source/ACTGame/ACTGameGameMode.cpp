// Copyright Epic Games, Inc. All Rights Reserved.

#include "ACTGameGameMode.h"
#include "ACTGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AACTGameGameMode::AACTGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

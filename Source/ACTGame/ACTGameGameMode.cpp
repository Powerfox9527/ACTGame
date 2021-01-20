// Copyright Epic Games, Inc. All Rights Reserved.

#include "ACTGameGameMode.h"
#include "ACTGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AACTGameGameMode::AACTGameGameMode()
{
	// set default pawn class to our Blueprinted character
	// 	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	// 	if (PlayerPawnBPClass.Class != NULL)
	// 	{
	// 		DefaultPawnClass = PlayerPawnBPClass.Class;
	// 	}
	// 	HeroClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/GASDocumentation/Characters/Hero/BP_HeroCharacter.BP_HeroCharacter_C"));
	// 	if (!HeroClass)
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find HeroClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	// 	}
}

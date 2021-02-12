// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAGHitReactDirection : uint8
{
	// 0
	None			UMETA(DisplayName = "None"),
	// 1
	Left 			UMETA(DisplayName = "Left"),
	// 2
	Front 			UMETA(DisplayName = "Front"),
	// 3
	Right			UMETA(DisplayName = "Right"),
	// 4
	Back			UMETA(DisplayName = "Back")
};

UENUM(BlueprintType)
enum class EACTAbilityInputID : uint8
{
	// 0 None
	None	UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm	UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel		UMETA(DisplayName = "Cancel"),
	// 3 J
	Attack		UMETA(DisplayName = "Attack"),
	// 4 K
	Roll		UMETA(DisplayName = "Roll"),
    // 5 L
	Command	UMETA(DisplayName = "Command"),
	// 6 I
	Style		UMETA(DisplayName = "Style"),
	// 7 Jump
	Jump			UMETA(DisplayName = "Jump")
};

UENUM(BlueprintType)
enum class ERoomDirection : uint8
{
	North = 0 				UMETA(DisplayName = "North"), // rotation = 0 (world forward)
	East = 255 				UMETA(DisplayName = "East"),  // rotation = -90 (world right)
	West = 1 				UMETA(DisplayName = "West"),  // rotation = 90 (world left)
	South = 2 				UMETA(DisplayName = "South"), // rotation = 180 (world backward)
	NbDirection = 4 		UMETA(Hidden)
};
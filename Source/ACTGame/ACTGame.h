// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

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

// Copyright Epic Games, Inc. All Rights Reserved.

#include "AGCharacterBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Ability/AGAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"

AAGCharacterBase::AAGCharacterBase()
{
	ASC = CreateDefaultSubobject<UAGAbilitySystemComponent>(TEXT("AbilitySystemObject"));
}

UAbilitySystemComponent* AAGCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AAGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GrantAbilities();
}

void AAGCharacterBase::GrantAbilities()
{
	for (TSubclassOf<UAGGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(StartupAbility));
	}
}

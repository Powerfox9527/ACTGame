// Copyright Epic Games, Inc. All Rights Reserved.

#include "AGCharacterBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"

AAGCharacterBase::AAGCharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponent>(TEXT("AbilitySystemObject"));
	AttributeSet = CreateDefaultSubobject<UAGAttributeSet>(TEXT("AttributeSet"));
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
}

UAbilitySystemComponent* AAGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GrantAbilities();
	AddStartupEffects();
}

void AAGCharacterBase::GrantAbilities()
{
	for (TSubclassOf<UAGGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility));
	}
}

void AAGCharacterBase::AddStartupEffects()
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		//后面要把1换成CharacterLevel
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

// void AAGCharacterBase::InitializeAttributes()
// {
// 	if (!AttributeSet)
// 	{
// 		return;
// 	}
// 
// 	if (!DefaultAttributes)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
// 		return;
// 	}
// 
// 	// Can run on Server and Client
// 	 FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
// 	 EffectContext.AddSourceObject(this);
// 	 
// 	 FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, AttributeSet->GetCharacterLevel(), EffectContext);
// 	 FActiveGameplayEffectHandle ActiveGEHandle;
// 	 if (NewHandle.IsValid())
// 	 {
// 	 	ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
// 	 }
// 	 bool flag = ActiveGEHandle.IsValid();
// }

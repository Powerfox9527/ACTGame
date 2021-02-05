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

	// Cache tags
	HitDirectionFrontTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"));
	HitDirectionBackTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Back"));
	HitDirectionRightTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Right"));
	HitDirectionLeftTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Left"));

	WindMagicTag = FGameplayTag::RequestGameplayTag(FName("Magic.Wind"));
	FireMagicTag = FGameplayTag::RequestGameplayTag(FName("Magic.Fire"));
	IceMagicTag = FGameplayTag::RequestGameplayTag(FName("Magic.Ice"));
	ThunderMagicTag = FGameplayTag::RequestGameplayTag(FName("Magic.Thunder"));

	DamageTextClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/UI/DamageTextComponent.DamageTextComponent_C"));

}

UAbilitySystemComponent* AAGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAGCharacterBase::ShowDamageNumber(float LocalDamageDone)
{
	UAGDamageTextWidgetComponent* DamageText = NewObject<UAGDamageTextWidgetComponent>(this, DamageTextClass);
	if (DamageText == nullptr)
		return;
	DamageText->RegisterComponent();
	DamageText->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	float DamageTextHeight = DamageText->GetComponentToWorld().GetTranslation().Z;
	DamageText->AddLocalOffset(FVector(0, 0, -1 * DamageTextHeight));
	DamageText->SetDamageText(LocalDamageDone);
}

EAGHitReactDirection AAGCharacterBase::GetHitReactDirection(const FVector& ImpactPoint)
{
	const FVector& ActorLocation = GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DistanceToRightLeftPlane >= 0)
		{
			return EAGHitReactDirection::Front;
		}
		else
		{
			return EAGHitReactDirection::Back;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			return EAGHitReactDirection::Right;
		}
		else
		{
			return EAGHitReactDirection::Left;
		}
	}

	return EAGHitReactDirection::Front;
}

bool AAGCharacterBase::IsAlive()
{
	return AttributeSet->GetHealth() > 0;
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

float AAGCharacterBase::GetHealth()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetHealth();
	}
	return 0;
}

float AAGCharacterBase::GetMaxHealth()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0;
}

float AAGCharacterBase::GetMana()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetMana();
	}
	return 0;
}

float AAGCharacterBase::GetMaxMana()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetMaxMana();
	}
	return 0;
}

float AAGCharacterBase::GetATB()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetATB();
	}
	return 0;
}

float AAGCharacterBase::GetMaxATB()
{
	if (AttributeSet != nullptr)
	{
		return 2.0f;
	}
	return 0;
}

float AAGCharacterBase::GetAttackPower()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetAttackPower();
	}
	return 0;
}

float AAGCharacterBase::GetDefensePower()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetDefensePower();
	}
	return 0;
}

float AAGCharacterBase::GetMagicPower()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetMagicPower();
	}
	return 0;
}

float AAGCharacterBase::GetMagicDefense()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetMagicDefense();
	}
	return 0;
}

float AAGCharacterBase::GetPower()
{
	if (AttributeSet != nullptr)
	{
		return AttributeSet->GetPower();
	}
	return 0;
}

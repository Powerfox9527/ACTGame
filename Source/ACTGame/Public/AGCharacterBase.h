// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystem/AGAttributeSet.h"
#include "CameraSystem/TargetSystemComponent.h"
#include "CameraSystem/TargetSystemTargetableInterface.h"
#include "UISystem/AGDamageTextWidgetComponent.h"
#include "ACTGame/ACTGame.h"

#include "AGCharacterBase.generated.h"

UCLASS(config=Game)
class AAGCharacterBase : public ACharacter, public IAbilitySystemInterface, public ITargetSystemTargetableInterface
{
	GENERATED_BODY()
public:
	AAGCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
		virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayHitReact(FGameplayTag HitDirection, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
		virtual void ShowDamageNumber(float LocalDamageDone);

	UFUNCTION(BlueprintCallable)
		void SpawnWeaponAndAttach();

	UFUNCTION(BlueprintCallable)
		EAGHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsBreaked = false;

	UFUNCTION(BlueprintCallable)
		bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
		bool IsAliveAndNoBreak() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<class UAGGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWeaponActor> WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AWeaponActor* Weapon;

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetHealth();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetMaxHealth();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetMana();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetMaxMana();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetATB();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetMaxATB();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetAttackPower();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetDefensePower();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetMagicPower();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetMagicDefense();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetPower();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetBreak();

	UFUNCTION(BlueprintCallable)
		void EnhancePower(float Enhancement);

	UFUNCTION(BlueprintCallable)
		void AccelerateATB(float Speed = 5.0f, float Time = 1.0f);

	void AccelerateATB_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UAGDamageTextWidgetComponent> DamageTextClass;

	bool IsTargetable_Implementation() const;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void RotateToActor(AActor* OtherActor, bool noPitch = true);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText SelfName;

	UFUNCTION(BlueprintCallable)
		bool IsPlayingMontage();

	UFUNCTION(BlueprintCallable)
		FTransform GetProjectileTransform();

	UFUNCTION(BlueprintCallable)
		virtual void GrantAbilities();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAGAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAGAttributeSet* AttributeSet;

	virtual void BeginPlay() override;

	virtual void AddStartupEffects();

	//old ATB Regen Rate
	float ATBRegenRate;

	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;
	FGameplayTag WindMagicTag;
	FGameplayTag FireMagicTag;
	FGameplayTag IceMagicTag;
	FGameplayTag ThunderMagicTag;
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;
};


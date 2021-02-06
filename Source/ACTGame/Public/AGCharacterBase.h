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
	AAGCharacterBase();

	UFUNCTION(BlueprintCallable)
		virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayHitReact(FGameplayTag HitDirection, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
		virtual void ShowDamageNumber(float LocalDamageDone);

	UFUNCTION(BlueprintCallable)
		EAGHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
		bool IsAlive() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UAGGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeaponActor> WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UAGDamageTextWidgetComponent> DamageTextClass;

	bool IsTargetable_Implementation() const;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void RotateToActor(AActor* OtherActor, bool noRoll = true);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText SelfName;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAGAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAGAttributeSet* AttributeSet;

	virtual void BeginPlay() override;

	virtual void GrantAbilities();
	virtual void AddStartupEffects();

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


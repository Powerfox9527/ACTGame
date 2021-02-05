// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystem/AGAttributeSet.h"
#include "ACTGame/ACTGame.h"
#include "AGCharacterBase.generated.h"

UCLASS(config=Game)
class AAGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAGCharacterBase();

	UFUNCTION(BlueprintCallable)
		virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
		virtual void PlayHitReact(FGameplayTag HitDirection, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
		virtual void ShowDamageNumber(float LocalDamageDone);

	UFUNCTION(BlueprintCallable)
		EAGHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
		bool IsAlive();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UAGGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;


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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAGAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAGAttributeSet* AttributeSet;

	virtual void BeginPlay() override;

	virtual void GrantAbilities();
	virtual void AddStartupEffects();
};


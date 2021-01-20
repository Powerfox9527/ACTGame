// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AGGameplayAbility.h"
#include "AGAbilitySystemComponent.h"
#include "AGCharacterBase.generated.h"

UCLASS(config=Game)
class AAGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAGCharacterBase();

	UFUNCTION(BlueprintCallable)
		virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void BeginPlay() override;
	void GrantAbilities();

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UAGGameplayAbility>> CharacterAbilities;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAGAbilitySystemComponent* ASC;
};


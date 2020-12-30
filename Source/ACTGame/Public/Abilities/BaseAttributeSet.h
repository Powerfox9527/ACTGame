// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ACTAbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *
 */
UCLASS()
class ACTGAME_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBaseAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
// 	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
// 	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

		UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)

		UPROPERTY(BlueprintReadOnly, Category = "Magic", ReplicatedUsing = OnRep_Magic)
		FGameplayAttributeData Magic;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Magic)

		UPROPERTY(BlueprintReadOnly, Category = "MaxMagic", ReplicatedUsing = OnRep_MaxMagic)
		FGameplayAttributeData MaxMagic;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMagic)

		UPROPERTY(BlueprintReadOnly, Category = "ATB", ReplicatedUsing = OnRep_ATB)
		FGameplayAttributeData ATB;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ATB)

		UPROPERTY(BlueprintReadOnly, Category = "MaxMagic", ReplicatedUsing = OnRep_MaxATB)
		FGameplayAttributeData MaxATB;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxATB)

		/** MoveSpeed affects how fast characters can move */
		UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
		FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MoveSpeed)

		UPROPERTY(BlueprintReadOnly, Category = "AttackPower", ReplicatedUsing = OnRep_AttackPower)
		FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AttackPower)

		UPROPERTY(BlueprintReadOnly, Category = "DefensePower", ReplicatedUsing = OnRep_DefensePower)
		FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, DefensePower)

protected:
	// These OnRep functions exist to make sure that the ability system internal representations are synchronized properly during replication
	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_Magic(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaxMagic(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_ATB(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaxATB(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_AttackPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_DefensePower(const FGameplayAttributeData& OldValue);
};

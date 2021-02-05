// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AGAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


class AAGCharacterBase;
/**
 * 
 */
UCLASS()
class ACTGAME_API UAGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	
	UAGAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "ATB")
		FGameplayAttributeData ATB;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, ATB)

	// ATB regen rate will passively increase Mana every second
	UPROPERTY(BlueprintReadOnly, Category = "ATB")
		FGameplayAttributeData ATBRegenRate;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, ATBRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, CharacterLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
		FGameplayAttributeData Break;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, Break)

	/** 
	 * 伤害计算公式为 (0.5+AT-DF)*POW*RAND
	 * AT为攻击力，DF为防御力，POW为每个能力特有的数值
	 * RAND范围为0.95到1.05，Break状态下0.5替换为2.5
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Fight")
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Fight")
		FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Fight")
		FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Fight")
		FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, MagicPower)

	UPROPERTY(BlueprintReadOnly, Category = "Fight")
		FGameplayAttributeData MagicDefense;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, MagicDefense)

	UPROPERTY(BlueprintReadOnly, Category = "Fight")
		FGameplayAttributeData Power;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, Power)
	
protected:
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

private:
	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/BaseAttributeSet.h"

UBaseAttributeSet::UBaseAttributeSet()
	:Health(100.0f),
	MaxHealth(100.0f),
	Magic(100.0f),
	MaxMagic(100.0f),
	ATB(0.0f),
	MaxATB(200.0f),
	AttackPower(1.0f),
	DefensePower(1.0f),
	MoveSpeed(1.0f)
{

}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

// 	DOREPLIFETIME(UBaseAttributeSet, Health);
// 	DOREPLIFETIME(UBaseAttributeSet, MaxHealth);
// 	DOREPLIFETIME(UBaseAttributeSet, Magic);
// 	DOREPLIFETIME(UBaseAttributeSet, MaxMagic);
// 	DOREPLIFETIME(UBaseAttributeSet, ATB);
// 	DOREPLIFETIME(UBaseAttributeSet, MaxATB);
// 	DOREPLIFETIME(UBaseAttributeSet, AttackPower);
// 	DOREPLIFETIME(UBaseAttributeSet, DefensePower);
// 	DOREPLIFETIME(UBaseAttributeSet, MoveSpeed);
}
void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldValue);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldValue);
}

void UBaseAttributeSet::OnRep_Magic(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Magic, OldValue);
}

void UBaseAttributeSet::OnRep_MaxMagic(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMagic, OldValue);
}

void UBaseAttributeSet::OnRep_ATB(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ATB, OldValue);
}

void UBaseAttributeSet::OnRep_MaxATB(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxATB, OldValue);
}

void UBaseAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, AttackPower, OldValue);
}

void UBaseAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, DefensePower, OldValue);
}

void UBaseAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MoveSpeed, OldValue);
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "AGDamageTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UAGDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetDamageText(float DamageAmount, FText text);
};

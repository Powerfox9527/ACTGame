// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Components/BoxComponent.h"
#include "AGMazeRoom.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class ACTGAME_API AAGMazeRoom : public ALevelScriptActor
{
	GENERATED_BODY()
public:
	// 房间的大小，用格子数目表示
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntVector RoomSize;
	// 一个格子的大小
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntVector RoomUnit;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform transform;

	virtual void Tick(float DeltaTime) override;

protected:
	bool bInited;
};

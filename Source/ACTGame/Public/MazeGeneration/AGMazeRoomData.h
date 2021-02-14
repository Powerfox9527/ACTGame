// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AGMazeRoomData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTGAME_API UAGMazeRoomData : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		FName RoomName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FIntVector RoomSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RoomDirection; //0123对应左下右上，先这样吧
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UWorld> Level;
};

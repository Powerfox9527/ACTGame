// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGeneration/AGMazeRoom.h"
#include "Kismet/GameplayStatics.h"
#include "MazeGeneration/AGMazeRoomData.h"
#include "AGMazeGenerator.generated.h"

USTRUCT(BlueprintType)
struct FMazePos
{
	GENERATED_USTRUCT_BODY()

public:
	int32 X;
	int32 Y;
	// 0墙，1还没遍历到的地方（造路用），2路，3房间，4房间与其他地方的链接点
	int32 Color;
};

USTRUCT(BlueprintType)
struct FMazeRow
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		TArray<FMazePos> Rooms;

	FMazePos& operator[](int32 i)
	{
		return Rooms[i];
	}

	void Add(int32 x, int32 y, int32 i)
	{
		FMazePos pos;
		pos.X = x;
		pos.Y = y;
		pos.Color = i;
		Rooms.Add(pos);
	}

	void Empty()
	{
		Rooms.Empty();
	}

	FString ToString()
	{
		FString str;
		for (int32 i = 0; i < Rooms.Num(); ++i)
		{
// 			str += FString::FromInt(Rooms[i].X);
// 			str += " ";
// 			str += FString::FromInt(Rooms[i].Y);
// 			str += " ";
			str += FString::FromInt(Rooms[i].Color);
		}
		return str;
	}

	int32 Num()
	{
		return Rooms.Num();
	}
};

USTRUCT(BlueprintType)
struct FMazeRoom
{
	GENERATED_USTRUCT_BODY()
public:
	FMazePos RoomStartPos;
	int32 RoomWidth;
	int32 RoomHeight;
};

UCLASS()
class ACTGAME_API AAGMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAGMazeGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FMazeRow> Maze;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<UAGMazeRoomData>> RoomLevels;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 SprinkleCount = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DeadEndLeftPercent = 0.8;

	// 一个格子的大小
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FIntVector RoomUnit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<AActor>> WallClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<AActor>> FloorClass;

	TArray<FMazePos> Path;
	TArray<FMazeRoom> Rooms;

public:
	//注意行数列数需是奇数
	UFUNCTION(BlueprintCallable)
		TArray<FMazeRow> GenerateLevel(int32 mazeWidth, int32 mazeHeight);

	UFUNCTION(BlueprintPure, BlueprintCallable)
		int32 GetColor(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable)
		void SetColor(int32 X, int32 Y, int32 Color);

	UFUNCTION(BlueprintCallable)
		void PrintMaze();

	UFUNCTION(BlueprintPure, BlueprintCallable)
		int32 GetWidth();
	UFUNCTION(BlueprintPure, BlueprintCallable)
		int32 GetHeight();
	UFUNCTION(BlueprintPure, BlueprintCallable)
		FTransform GetTileTransform(int32 X, int32 Y, int32 Direction);
	UFUNCTION(BlueprintCallable)
		void SpawnWallsAndFloor();

protected:

	void InitMaze(int32 mazeWidth, int32 mazeHeight);

	bool IsValidPos(int32 X, int32 Y, TArray<int32> ValidColor);
	
	bool IsOutOfBound(int32 X, int32 Y);

	void DFS(int32 X, int32 Y);

	void ConnectRooms();

	void Sprinkle(int32 TryCount);

	void CheckRooms();

	void ClearDeadEnds(float LeftPercent);

	bool IsDeadEnd(int32 i, int32 j);
	//这个函数用来找下一个要生成路的点，返回坐标
	FMazePos FindNextPoint(int32 X, int32 Y, int32& Direction, TArray<int32> ValidColor);
};

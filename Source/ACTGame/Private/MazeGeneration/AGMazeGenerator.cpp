// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGeneration/AGMazeGenerator.h"
#include "Engine/LevelStreaming.h"

// Sets default values
AAGMazeGenerator::AAGMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAGMazeGenerator::BeginPlay()
{
	//Super::BeginPlay();
	GenerateLevel(15, 15);
}

// Called every frame
void AAGMazeGenerator::Tick(float DeltaTime)
{
}

TArray<FMazeRow> AAGMazeGenerator::GenerateLevel(int32 mazeWidth, int32 mazeHeight)
{
	 InitMaze(mazeWidth, mazeHeight);
	 Path.Add(Maze[1][1]);
	 //DFS(1, 1);
	 Sprinkle(15);
	 PrintMaze();
	 
	 return Maze;
}

void AAGMazeGenerator::InitMaze(int32 mazeWidth, int32 mazeHeight)
{
	if (mazeHeight <= 2 || mazeHeight % 2 == 0 || mazeWidth % 2 == 0)
		return;
	TArray<FMazeRow> resArray;
	//先初始化迷宫，用1表示需要生成路线的地方
	//单数行全是0

	for (int32 i = 0; i < mazeHeight; ++i)
	{
		FMazeRow Row;
		if (i % 2 == 0)
		{
			for (int32 j = 0; j < mazeWidth; ++j)
			{
				Row.Add(i, j, 0);
			}
		}
		else
		{
			for (int32 j = 0; j < mazeWidth; ++j)
			{
				if (j % 2 == 1)
				{
					Row.Add(i, j, 1);
				}
				else
				{
					Row.Add(i, j, 0);
				}
			}
		}
		resArray.Add(Row);
	}

	Maze = resArray;
}

bool AAGMazeGenerator::IsValidPos(int32 X, int32 Y)
{
	// 这个小于2的后面要改
	return !IsOutOfBound(X, Y) && Maze[X][Y].Color < 2;
}

bool AAGMazeGenerator::IsOutOfBound(int32 X, int32 Y)
{
	return !(X < Maze.Num() && X >= 0 && Y >= 0 && Y < Maze[0].Num());
}

void AAGMazeGenerator::PrintMaze()
{
	for (int32 i = 0; i < Maze.Num(); ++i)
	{
		FString str = Maze[i].ToString();
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, *str);
	}
}

void AAGMazeGenerator::DFS(int32 X, int32 Y)
{
	if(IsOutOfBound(X, Y))
		return;
	int32 Direction;
	Maze[X][Y].Color = 2;
	FMazePos NextPos = FindNextPoint(X, Y, Direction);
	if (NextPos.X == -1 && NextPos.Y == -1)
	{
		if (Path.Num() == 0)
			return;
		FMazePos PrePos = Path.Pop();
		DFS(PrePos.X, PrePos.Y);
		return;
	}
	// 代表该点的四个方位，记住中间要隔一格再取, 顺序是左下右上
	TArray<int32> XOffset = { -1, 0, 1, 0 };
	TArray<int32> YOffset = { 0, -1, 0, 1 };
	Maze[X+XOffset[Direction]][Y+YOffset[Direction]].Color = 2;
	Path.Add(Maze[X][Y]);
	DFS(NextPos.X, NextPos.Y);
}

int32 AAGMazeGenerator::GetColor(int32 X, int32 Y)
{
	return Maze[X][Y].Color;
}

void AAGMazeGenerator::Sprinkle(int32 TryCount)
{
// 此处要加上对没有房间和房间边长为偶数的情况的判断
	for (int32 k = 0; k < TryCount; ++k)
	{
// 		TSubclassOf<AAGMazeRoom> RoomClass = RoomClasses[FMath::RandRange(0, RoomClasses.Num())];
// 		AAGMazeRoom* Room = RoomClass.GetDefaultObject();
		FIntVector RoomSize = {3, 3, 3};
		if(RoomSize.X % 2 == 0 || RoomSize.Y % 2 == 0)
			continue;
		// 设置关卡生成时的方向, 暂时略
		// 关卡生成点需要是单数，行数列数都要
		TArray<int32> RoomStartPosX;
		TArray<int32> RoomStartPosY;
		for (int32 l = 0; l < Maze.Num(); ++l)
		{
			if(l % 2 == 1 && RoomSize.X + l < Maze.Num())
				RoomStartPosX.Add(l);
		}
		for (int32 l = 0; l < Maze[0].Num(); ++l)
		{
			if (l % 2 == 1 && RoomSize.Y + l < Maze[0].Num())
				RoomStartPosY.Add(l);
		}
		int32 RoomStartX = RoomStartPosX[FMath::FRandRange(0, RoomStartPosX.Num())];
		int32 RoomStartY = RoomStartPosY[FMath::FRandRange(0, RoomStartPosY.Num())];
		bool bOverlaped = false;
		for (int32 i = RoomStartX; i < RoomStartX + RoomSize.X; ++i)
		{
			for (int32 j = RoomStartY; j < RoomStartY + RoomSize.Y; ++j)
			{
				if (GetColor(i, j) >= 2)
				{
					bOverlaped = true;
				}
			}
		}
		if (!bOverlaped)
		{
			for (int32 i = RoomStartX; i < RoomStartX + RoomSize.X; ++i)
			{
				for (int32 j = RoomStartY; j < RoomStartY + RoomSize.Y; ++j)
				{
					Maze[i][j].Color = 2;
				}
			}
		}
	}
}

FMazePos AAGMazeGenerator::FindNextPoint(int32 X, int32 Y, int32& Direction)
{
	// 代表该点的四个方位，记住中间要隔一格再取, 顺序是左下右上
	TArray<int32> XOffset = { -2, 0, 2, 0 };
	TArray<int32> YOffset = { 0, -2, 0, 2 };
	//把可以生成的点放在一个数组中，随机取一个方向
	TArray<int32> CandidatePos;
	for (int32 i = 0; i < 4; ++i)
	{
		if (IsValidPos(X + XOffset[i], Y + YOffset[i]))
		{
			CandidatePos.Add(i);
		}
	}
	FMazePos Res;
	if (CandidatePos.Num() <= 0)
	{
		Res.X = -1;
		Res.Y = -1;
		Res.Color = 2;
		Direction = -1;
		return Res;
	}
	Direction = CandidatePos[FMath::FRandRange(0, CandidatePos.Num())];
	Res.X = X + XOffset[Direction];
	Res.Y = Y + YOffset[Direction];
	return Res;
}


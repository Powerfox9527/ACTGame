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
	Super::BeginPlay();
}

// Called every frame
void AAGMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<FMazeRow> AAGMazeGenerator::GenerateLevel(int32 mazeWidth, int32 mazeHeight)
{
	if (mazeHeight % 2 == 0)
	{
		mazeHeight++;
	}
	if (mazeWidth % 2 == 0)
	{
		mazeWidth++;
	}
	 InitMaze(mazeWidth, mazeHeight);
	 Path.Add(Maze[1][1]);
	 Sprinkle(SprinkleCount);
	 DFS(1, 1);
	 ConnectRooms();
	 //PrintMaze();
	 
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

bool AAGMazeGenerator::IsValidPos(int32 X, int32 Y, TArray<int32> ValidColor)
{
	
	return !IsOutOfBound(X, Y) && ValidColor.Contains(Maze[X][Y].Color);
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

int32 AAGMazeGenerator::GetWidth()
{
	return Maze.Num();
}

int32 AAGMazeGenerator::GetHeight()
{
	if(Maze.Num() == 0)
		return 0;
	return Maze[0].Num();
}

FTransform AAGMazeGenerator::GetTileTransform(int32 X, int32 Y, int32 Direction)
{
	FTransform resTransform;
	if (IsOutOfBound(X, Y))
	{
		return resTransform;
	}
	FVector Location = GetActorLocation();
	Location.X += X * RoomUnit.X;
	Location.Y += Y * RoomUnit.Y;
	resTransform.SetLocation(Location);
	return resTransform;
}

void AAGMazeGenerator::DFS(int32 X, int32 Y)
{
	if(IsOutOfBound(X, Y))
		return;
	int32 Direction;
	Maze[X][Y].Color = 2;
	TArray<int32> DFSValidColors = {0, 1};
	FMazePos NextPos = FindNextPoint(X, Y, Direction, DFSValidColors);
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

void AAGMazeGenerator::ConnectRooms()
{
	for(FMazeRoom room : Rooms)
	{
		TArray<int32> ConnectCandidateX;
		TArray<int32> ConnectCandidateY;
		for (int32 i = room.RoomStartPos.X; i < room.RoomStartPos.X + room.RoomWidth; ++i)
		{
			for (int32 j = room.RoomStartPos.Y; j < room.RoomStartPos.Y + room.RoomHeight; ++j)
			{
				TArray<int32> ConnectValidPos = {2};
				int32 Direction;
				// 代表该点的四个方位，记住中间要隔一格再取, 顺序是左下右上
				TArray<int32> XOffset = { -1, 0, 1, 0 };
				TArray<int32> YOffset = { 0, -1, 0, 1 };
				if (FindNextPoint(i, j, Direction, ConnectValidPos).X != -1)
				{
					ConnectCandidateX.Add(i + XOffset[Direction]);
					ConnectCandidateY.Add(j + YOffset[Direction]);
				}
			}
		}

		if (ConnectCandidateX.Num() != 0)
		{
			int32 index = FMath::RandRange(0, ConnectCandidateX.Num() - 1);
			Maze[ConnectCandidateX[index]][ConnectCandidateY[index]].Color = 2;
		}
	}
}

int32 AAGMazeGenerator::GetColor(int32 X, int32 Y)
{
	if(GetWidth() == 0 || GetHeight() == 0)
		return -1;
	return Maze[X][Y].Color;
}

void AAGMazeGenerator::Sprinkle(int32 TryCount)
{
	if(RoomLevels.Num() == 0)
		return;

	for (int32 k = 0; k < TryCount; ++k)
	{
//  	TSubclassOf<UAGMazeRoomData> RoomClass = RoomLevels[FMath::RandRange(0, RoomLevels.Num()-1)];
// 		UAGMazeRoomData* Room = RoomClass.GetDefaultObject();
		FIntVector RoomSize = {3, 3, 1};
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
		if(RoomStartPosX.Num() == 0 || RoomStartPosY.Num() == 0)
			continue;
		int32 RoomStartX = RoomStartPosX[FMath::RandRange(0, RoomStartPosX.Num()-1)];
		int32 RoomStartY = RoomStartPosY[FMath::RandRange(0, RoomStartPosY.Num()-1)];
		bool bOverlaped = false;
		for (int32 i = RoomStartX; i < RoomStartX + RoomSize.X; ++i)
		{
			for (int32 j = RoomStartY; j < RoomStartY + RoomSize.Y; ++j)
			{
				if (GetColor(i, j) >= 3)
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
					Maze[i][j].Color = 3;
				}
			}
			FMazeRoom room;
			room.RoomStartPos = Maze[RoomStartX][RoomStartY];
			room.RoomWidth = RoomSize.X;
			room.RoomHeight = RoomSize.Y;
			Rooms.Add(room);
		}

	}
}

FMazePos AAGMazeGenerator::FindNextPoint(int32 X, int32 Y, int32& Direction, TArray<int32> ValidColor)
{
	// 代表该点的四个方位，记住中间要隔一格再取, 顺序是左下右上
	TArray<int32> XOffset = { -2, 0, 2, 0 };
	TArray<int32> YOffset = { 0, -2, 0, 2 };
	//把可以生成的点放在一个数组中，随机取一个方向
	TArray<int32> CandidatePos;
	for (int32 i = 0; i < 4; ++i)
	{
		if (IsValidPos(X + XOffset[i], Y + YOffset[i], ValidColor))
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
	Direction = CandidatePos[FMath::RandRange(0, CandidatePos.Num()-1)];
	Res.X = X + XOffset[Direction];
	Res.Y = Y + YOffset[Direction];
	return Res;
}


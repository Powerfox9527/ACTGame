// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGeneration/AGMazeGenerator.h"
#include "Engine/LevelStreaming.h"
#include "MazeGeneration/AGProceduralLevelStreaming.h"
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
	Sprinkle(SprinkleCount);
	while (true)
	{
		TArray<int32> CandidatePosX;
		TArray<int32> CandidatePosY;
		for (int32 i = 0; i < GetWidth(); ++i)
		{
			for (int32 j = 0; j < GetHeight(); ++j)
			{
				if (GetColor(i, j) == 1)
				{
					CandidatePosX.Add(i);
					CandidatePosY.Add(j);
				}
			}
		}

		if (CandidatePosX.Num() > 0)
		{
			int32 index = FMath::RandRange(0, CandidatePosX.Num() - 1);
			int32 x = CandidatePosX[index];
			int32 y = CandidatePosY[index];
			Path.Empty();
			Path.Add(Maze[x][y]);
			DFS(x, y);
		}
		else
		{
			break;
		}
	}
	 ConnectRooms();
	 CheckRooms();
	 ClearDeadEnds(DeadEndLeftPercent);
	 SpawnExit();
	 SpawnWallsAndFloor();
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

void AAGMazeGenerator::SpawnWallsAndFloor()
{
	for (int32 i = 0; i < GetWidth(); ++i)
	{
		for (int32 j = 0; j < GetHeight(); ++j)
		{
			FTransform transform = GetTileTransform(i, j, 0);
			FActorSpawnParameters SpawnInfo;
			if (GetColor(i, j) == 0)
			{
				TSubclassOf<AActor> Wall = WallClass[FMath::RandRange(0, WallClass.Num()-1)];
				GetWorld()->SpawnActor<AActor>(Wall, transform);
			}
			else if(GetColor(i, j) == 2)
			{
				TSubclassOf<AActor> Floor = FloorClass[FMath::RandRange(0, FloorClass.Num()-1)];
				GetWorld()->SpawnActor<AActor>(Floor, transform);
			}
		}
	}
}

void AAGMazeGenerator::SpawnExit()
{
	int32 ExitX = Maze.Num() - 1;
	int32 ExitY = FMath::RandRange(1, Maze[0].Num() - 2);
	ExitPos = Maze[ExitX][ExitY];
	SetColor(ExitX, ExitY, 5);
	FTransform transform = GetTileTransform(ExitX, ExitY, 0);
	GetWorld()->SpawnActor<AActor>(ExitClass, transform);
}

void AAGMazeGenerator::DFS(int32 X, int32 Y)
{
	if(IsOutOfBound(X, Y))
		return;
	int32 Direction;
	SetColor(X, Y, 2);
	TArray<int32> DFSValidColors = {1};
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
	SetColor(X + XOffset[Direction], Y + YOffset[Direction], 2);
	Path.Add(Maze[X][Y]);
	DFS(NextPos.X, NextPos.Y);
}

void AAGMazeGenerator::ConnectRooms()
{
	for(FMazeRoom room : Rooms)
	{
		TArray<int32> ConnectCandidateX;
		TArray<int32> ConnectCandidateY;
		//检查有没有可以连接到走廊或房间的点
		for (int32 i = room.RoomStartPos.X; i < room.RoomStartPos.X + room.RoomWidth; ++i)
		{
			int32 Color = GetColor(i, room.RoomStartPos.Y - 2);
			if (Color == 2 || Color == 3)
			{
				ConnectCandidateX.Add(i);
				ConnectCandidateY.Add(room.RoomStartPos.Y - 1);
			}
		}
		if (ConnectCandidateX.Num() > 0)
		{
			int32 index = FMath::RandRange(0, ConnectCandidateX.Num()-1);
			SetColor(ConnectCandidateX[index], ConnectCandidateY[index], 2);
		}

		ConnectCandidateX.Empty();
		ConnectCandidateY.Empty();
		for (int32 i = room.RoomStartPos.X; i < room.RoomStartPos.X + room.RoomWidth; ++i)
		{
			//注意这里最高处是room.RoomStartPos.Y + room.RoomHeight - 1
			int32 Color = GetColor(i, room.RoomStartPos.Y + room.RoomHeight + 1);
			if (Color == 2 || Color == 3)
			{
				ConnectCandidateX.Add(i);
				ConnectCandidateY.Add(room.RoomStartPos.Y + room.RoomHeight);
			}
		}
		if (ConnectCandidateX.Num() > 0)
		{
			int32 index = FMath::RandRange(0, ConnectCandidateX.Num() - 1);
			SetColor(ConnectCandidateX[index], ConnectCandidateY[index], 2);
		}
		
		ConnectCandidateX.Empty();
		ConnectCandidateY.Empty();
		for (int32 j = room.RoomStartPos.Y; j < room.RoomStartPos.Y + room.RoomHeight; ++j)
		{
			int32 Color = GetColor(room.RoomStartPos.X - 2, j);
			if (Color == 2 || Color == 3)
			{
				ConnectCandidateX.Add(room.RoomStartPos.X - 1);
				ConnectCandidateY.Add(j);
			}
		}
		if (ConnectCandidateX.Num() > 0)
		{
			int32 index = FMath::RandRange(0, ConnectCandidateX.Num() - 1);
			SetColor(ConnectCandidateX[index], ConnectCandidateY[index], 2);
		}

		ConnectCandidateX.Empty();
		ConnectCandidateY.Empty();
		for (int32 j = room.RoomStartPos.Y; j < room.RoomStartPos.Y + room.RoomHeight; ++j)
		{
			int32 Color = GetColor(room.RoomStartPos.X  + room.RoomWidth + 1, j);
			if (Color == 2 || Color == 3)
			{
				ConnectCandidateX.Add(room.RoomStartPos.X + room.RoomWidth);
				ConnectCandidateY.Add(j);
			}
		}
		if (ConnectCandidateX.Num() > 0)
		{
			int32 index = FMath::RandRange(0, ConnectCandidateX.Num() - 1);
			SetColor(ConnectCandidateX[index], ConnectCandidateY[index], 2);
		}
	}
}

int32 AAGMazeGenerator::GetColor(int32 X, int32 Y)
{
	if(IsOutOfBound(X, Y))
		return -1;
	return Maze[X][Y].Color;
}

void AAGMazeGenerator::SetColor(int32 X, int32 Y, int32 Color)
{
	if (!IsOutOfBound(X, Y))
	{
		Maze[X][Y].Color = Color;
	}
}

void AAGMazeGenerator::Sprinkle(int32 TryCount)
{
	if(RoomLevels.Num() == 0)
		return;

	for (int32 k = 0; k < TryCount; ++k)
	{
	  	TSubclassOf<UAGMazeRoomData> RoomClass = RoomLevels[FMath::RandRange(0, RoomLevels.Num()-1)];
		UAGMazeRoomData* Room = RoomClass.GetDefaultObject();
		FIntVector RoomSize = Room->RoomSize;
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
					SetColor(i, j, 3);
				}
			}
			FMazeRoom room;
			room.RoomStartPos = Maze[RoomStartX][RoomStartY];
			room.RoomWidth = RoomSize.X;
			room.RoomHeight = RoomSize.Y;
			Rooms.Add(room);
			FTransform transform = GetTileTransform(RoomStartX, RoomStartY, 0);
			UAGProceduralLevelStreaming::Load(GetWorld(), Room, transform.GetLocation(), transform.GetRotation().Rotator());
		}

	}
}

void AAGMazeGenerator::CheckRooms()
{
	for (FMazeRoom room : Rooms)
	{
		for (int32 i = room.RoomStartPos.X; i < room.RoomStartPos.X + room.RoomWidth; ++i)
		{
			for (int32 j = room.RoomStartPos.Y; j < room.RoomStartPos.Y + room.RoomHeight; ++j)
			{
				//检查是否此处被标为2
				if (GetColor(i, j) == 2)
				{
					SetColor(i, j,3);
				}
			}
		}
	}
}

void AAGMazeGenerator::ClearDeadEnds(float LeftPercent)
{
	int32 CorridorNum = 0;
	if(LeftPercent == 0)
		return;
	TArray<int32> DeadEndX;
	TArray<int32> DeadEndY;
	for (int32 i = 0; i < GetWidth(); ++i)
	{
		for (int32 j = 0; j < GetHeight(); ++j)
		{
			if (GetColor(i, j) == 2)
			{
				CorridorNum++;
			}
		}
	}
	if(CorridorNum == 0)
		return;
	int32 DissolveCount = 0;
	while (true)
	{
		DeadEndX.Empty();
		DeadEndY.Empty();
		for (int32 i = 0; i < GetWidth(); i++)
		{
			for (int32 j = 0; j < GetHeight(); ++j)
			{
				if (IsDeadEnd(i, j))
				{
					DeadEndX.Add(i);
					DeadEndY.Add(j);
				}
			}
		}

		if(DissolveCount >= (1 - LeftPercent) * CorridorNum || DeadEndX.Num() == 0)
			break;
		else
		{
			int32 tempDissolveCount = 0;
			int32 index = FMath::RandRange(0, DeadEndX.Num() - 1);
			int32 X = DeadEndX[index];
			int32 Y = DeadEndY[index];
			SetColor(X, Y, 0);
			DissolveCount++;
		}
	}
}

bool AAGMazeGenerator::IsDeadEnd(int32 i, int32 j)
{
	if(IsOutOfBound(i, j) || GetColor(i, j) == 0)
		return false;
	TArray<int32> XOffset = { -1, 0, 1, 0 };
	TArray<int32> YOffset = { 0, -1, 0, 1 };
	int32 WallCount = 0;
	for (int32 k = 0; k < 4; ++k)
	{
		if (!IsOutOfBound(i + XOffset[k], j + YOffset[k]) && GetColor(i + XOffset[k], j + YOffset[k]) == 0)
		{
			WallCount++;
		}
	}
	if(WallCount >= 3)
		return true;
	return false;
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


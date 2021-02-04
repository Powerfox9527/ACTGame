// Fill out your copyright notice in the Description page of Project Settings.


#include "AISystem/BTT_RoundCharacter.h"

FVector UBTT_RoundCharacter::GetNewRoundLocation(FVector OriginLocation, FVector NowLocation)
{
	FVector VectorToSelf = NowLocation - OriginLocation;
	float Radius = VectorToSelf.Size();
	float Angle = FMath::RandRange(1.0f, 20.0f);
	FRotator rot(0, Angle, 0);
	VectorToSelf.Normalize();
	FVector VecToNewSelf = rot.RotateVector(VectorToSelf);
	VecToNewSelf = VecToNewSelf * Radius / VecToNewSelf.Size();
	FVector NewLocation = OriginLocation + VecToNewSelf;
	return NewLocation;
}

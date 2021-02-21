// Fill out your copyright notice in the Description page of Project Settings.


#include "AGBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ACTGame/Public/ACTGameCharacter.h"

AActor* UAGBlueprintFunctionLibrary::GetClosetActorOfClass(AActor* Origin, TSubclassOf<AActor> ActorClass)
{
	if(Origin == nullptr)
		return nullptr;
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(Origin->GetWorld(), ActorClass, Actors);
	float Distance = INT_MAX;
	AActor* returnActor = nullptr;
	for (AActor* a : Actors)
	{
		if(Origin == a)
			continue;
		float tempDistance = a->GetDistanceTo(Origin);
		if (tempDistance < Distance)
		{
			Distance = tempDistance;
			returnActor = a;
		}
	}
	return returnActor;
}

bool UAGBlueprintFunctionLibrary::IsBothPlayer(ACharacter* CharacterA, ACharacter* CharacterB)
{
	AACTGameCharacter* chA = Cast<AACTGameCharacter>(CharacterA);
	AACTGameCharacter* chB = Cast<AACTGameCharacter>(CharacterB);
	if ((chA != nullptr && chA != nullptr) || (chA == chB))
	{
		return true;
	}
	return false;
}

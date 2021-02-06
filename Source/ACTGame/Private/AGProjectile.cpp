// Fill out your copyright notice in the Description page of Project Settings.


#include "AGProjectile.h"

// Sets default values
AAGProjectile::AAGProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AAGProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}



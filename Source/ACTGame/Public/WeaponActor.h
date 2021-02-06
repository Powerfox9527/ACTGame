// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayAbilities/Public/GameplayEffect.h"
#include "AGCharacterBase.h"
#include "WeaponActor.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UGameplayEffect;
class AAGCharacterBase;

UCLASS()
class ACTGAME_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Collider")
		UCapsuleComponent* AttachPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Collider")
		USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Collider")
		UCapsuleComponent* WeaponCollider;
 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Effect")
 		TArray<TSubclassOf<UGameplayEffect>> WeaponEffects;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Character")
		AAGCharacterBase* OwningCharacter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Control")
		bool CanAttack;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void LagFrame(int32 LagCount, float LagTime);

	UFUNCTION(BlueprintCallable)
	FTransform GetProjectileTransform();
};

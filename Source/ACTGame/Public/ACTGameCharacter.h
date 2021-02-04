// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponActor.h"
#include "AGCharacterBase.h"
#include "ACTGameCharacter.generated.h"


UCLASS(config=Game)
class AACTGameCharacter : public AAGCharacterBase
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AACTGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable)
		void Attack();
	UFUNCTION(BlueprintCallable)
		void Command();
	UFUNCTION(BlueprintCallable)
		void Roll();
	UFUNCTION(BlueprintCallable)
		void Style();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack" )
	FName ComboSectionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeaponActor> WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponActor* Weapon;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	virtual void BeginPlay() override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	virtual void Tick(float deltaTime) override;

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetMana();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetMaxMana();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetATB();
	
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetMaxATB();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	TArray<float> GetUIAttributeData();
};


// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponActor.h"
#include "AGCharacterBase.h"
#include "CameraSystem/TargetSystemComponent.h"
#include "ACTGameCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeStyleDelegate, FText, StyleName, bool, Styled);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UTargetSystemComponent* TargetComponent;

public:
	AACTGameCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool ActivateStyle();
		bool ActivateStyle_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool DeactivateStyle();
		bool DeactivateStyle_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ComboSectionName;

	UFUNCTION(BlueprintCallable)
		void SetAbilityTarget(AAGCharacterBase* OtherCharacter);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool Styled;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText StyleName;

	UPROPERTY(BlueprintAssignable)
	FChangeStyleDelegate ChangeStyle;

	UFUNCTION(BlueprintCallable)
	bool IsAIControll();

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

	virtual void UnPossessed() override;

	void LockOn();

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
	TArray<float> GetUIAttributeData();

};


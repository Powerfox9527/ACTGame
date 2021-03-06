// Copyright Epic Games, Inc. All Rights Reserved.

#include "ACTGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AIModule/Classes/AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ACTGame/ACTGameGameMode.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AACTGameCharacter

AACTGameCharacter::AACTGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	GetCharacterMovement()->MaxAcceleration = 1000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 100.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->ProbeChannel = ECollisionChannel::ECC_Visibility;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TargetComponent = CreateDefaultSubobject<UTargetSystemComponent>(TEXT("TargetSystem"));
	TargetComponent->PitchMax = 10.0f;
	TargetComponent->PitchMin = -10.0f;
	TargetComponent->bShouldControlRotation = false;
	TargetComponent->bAdjustPitchBasedOnDistanceToTarget = false;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AACTGameCharacter::Attack()
{
	if (AbilitySystemComponent)
	{
		if (TargetComponent->LockedOnTargetActor != nullptr)
		{
			RotateToActor(TargetComponent->LockedOnTargetActor);
			AbilityTarget = Cast<AAGCharacterBase>(TargetComponent->LockedOnTargetActor);
		}
		FGameplayTag tag = FGameplayTag::RequestGameplayTag(FName("AbilityInputID.Attack"));
		FGameplayTagContainer tagContainer(tag);
		AbilitySystemComponent->TryActivateAbilitiesByTag(tagContainer);
	}
}

void AACTGameCharacter::Command()
{
	if (AbilitySystemComponent)
	{
		FGameplayTag tag = FGameplayTag::RequestGameplayTag(FName("AbilityInputID.Command"));
		FGameplayTagContainer tagContainer(tag);
		AbilitySystemComponent->TryActivateAbilitiesByTag(tagContainer);
	}
}

void AACTGameCharacter::Roll()
{
	if (AbilitySystemComponent)
	{
		FGameplayTag tag = FGameplayTag::RequestGameplayTag(FName("AbilityInputID.Roll"));
		FGameplayTagContainer tagContainer(tag);
		AbilitySystemComponent->TryActivateAbilitiesByTag(tagContainer);
	}
}

void AACTGameCharacter::Style()
{
	if (Styled)
	{
		DeactivateStyle();
		Styled = false;
	}
	else
	{
		ActivateStyle();
		Styled = true;
	}
}

bool AACTGameCharacter::ActivateStyle_Implementation()
{
	Styled = true;
	return Styled;
}

bool AACTGameCharacter::DeactivateStyle_Implementation()
{
	Styled = false;
	return Styled;
}

void AACTGameCharacter::SetAbilityTarget(AAGCharacterBase* OtherCharacter)
{
	if(OtherCharacter == nullptr)
		return;
	RotateToActor(OtherCharacter);
	AbilityTarget = OtherCharacter;
}

bool AACTGameCharacter::IsAIControll()
{
	return (Controller != nullptr) && Cast<AAIController>(Controller) != nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AACTGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
// 	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
// 	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AACTGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AACTGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AACTGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AACTGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AACTGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AACTGameCharacter::TouchStopped);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AACTGameCharacter::Attack);
	PlayerInputComponent->BindAction("Command", IE_Pressed, this, &AACTGameCharacter::Command);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AACTGameCharacter::Roll);
	PlayerInputComponent->BindAction("Style", IE_Pressed, this, &AACTGameCharacter::Style);
	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &AACTGameCharacter::LockOn);
}

void AACTGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AACTGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AACTGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Spwan WeaponActor
	SpawnWeaponAndAttach();
}

void AACTGameCharacter::UnPossessed()
{
	Super::UnPossessed();
	AACTGameGameMode* GameMode = Cast<AACTGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode->MainCharacter != this && GameMode->MainCharacter != nullptr)
	{
		SpawnDefaultController();
		TargetComponent->TargetLockOff();
	}
}

void AACTGameCharacter::LockOn()
{
	TargetComponent->TargetActor();
	AbilityTarget = Cast<AAGCharacterBase>(TargetComponent->GetLockedOnTargetActor());
}

void AACTGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AACTGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AACTGameCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !IsPlayingMontage())
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AACTGameCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !IsPlayingMontage())
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AACTGameCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	if (IsAIControll())
	{
		//AbilityTarget = UAGBlueprintFunctionLibrary::GetClosetActorOfClass(this, AAGEnemyBase::GetClass());
		//RotateToActor(AbilityTarget);
	}
	else if (!IsPlayingMontage())
	{
		AbilityTarget = Cast<AAGCharacterBase>(TargetComponent->LockedOnTargetActor);
	}
}

TArray<float> AACTGameCharacter::GetUIAttributeData()
{
	TArray<float> res;
	float HealthPercent = GetMaxHealth() != 0 ? GetHealth() / GetMaxHealth() : 0;
	res.Add(HealthPercent);
	float ManaPercent = GetMaxMana() != 0 ? GetMana() / GetMaxMana() : 0;
	res.Add(ManaPercent);
	float ATBPercent1 = GetATB() > 1 ? 1 : GetATB();
	res.Add(ATBPercent1);
	float ATBPercent2 = GetATB() > 1 ? GetATB() - 1 : 0;
	res.Add(ATBPercent2);
	return res;
}
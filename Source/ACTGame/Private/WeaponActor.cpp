// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "AbilitySystemComponent.h"
#include "AGCharacterBase.h"
// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttachPoint = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttachPoint"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollider"));

	Mesh->SetupAttachment(AttachPoint);
	WeaponCollider->SetupAttachment(AttachPoint);

	WeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::BeginOverlap);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != OwningCharacter && CanAttack)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Other Get Weapon Hit"));
		AAGCharacterBase* EnmeyActor = Cast<AAGCharacterBase>(OtherActor);
		if (EnmeyActor)
		{
			UAbilitySystemComponent *AbilitySystemComponent = OwningCharacter->GetAbilitySystemComponent();
			for (TSubclassOf<UGameplayEffect> effectClass : WeaponEffects)
			{
				FGameplayEffectSpec spec;
				UGameplayEffect* effect = effectClass.GetDefaultObject();
				spec.Initialize(effect, AbilitySystemComponent->MakeEffectContext());
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(spec, EnmeyActor->GetAbilitySystemComponent());
			}
		}
	}
}

void AWeaponActor::LagFrame(int32 LagCount, float LagTime)
{

}

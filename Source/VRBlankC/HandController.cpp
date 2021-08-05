// Fill out your copyright notice in the Description page of Project Settings.


#include "HandController.h"
#include "MotionControllerComponent.h"
#include "Components/SphereComponent.h"

#include "VRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHandController::AHandController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Ma"));
	SetRootComponent(MotionController);
	MotionController->bDisplayDeviceModel = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Esfera de Collision"));
	SphereCollision->SetupAttachment(MotionController);
	SphereCollision->SetSphereRadius(4.5f);
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	bCanClimb = false;
	bIsClimbing = false;
}

// Called when the game starts or when spawned
void AHandController::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AHandController::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AHandController::ActorEndOverlap);

	
}

// Called every frame
void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsClimbing) {
		FVector Delta = GetActorLocation() - ClimingStartLocation;
		GetAttachParentActor()->AddActorWorldOffset(-Delta);
	}
}

void AHandController::SetHand(FName Hand)
{
	if (MotionController != nullptr)
	{
		MotionController->SetTrackingMotionSource(Hand);
	}

}

void AHandController::Grip()
{
	if (!bCanClimb) return;

	if (!bIsClimbing)
	{
		bIsClimbing = true;
		ClimingStartLocation = GetActorLocation();

		OtherController->bIsClimbing = false;

		AVRCharacter* Papa = Cast<AVRCharacter>(GetAttachParentActor());
		if (Papa != nullptr)
		{
			Papa->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		}
	}
}

void AHandController::Release()
{
	if (bIsClimbing)
	{
		bIsClimbing = false;
		AVRCharacter* Papa = Cast<AVRCharacter>(GetAttachParentActor());
		if (Papa != nullptr)
		{
			Papa->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}
	}
}

void AHandController::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	bool bNew = CanClimb();
	if (!bCanClimb && bNew)
	{
		UE_LOG(LogTemp, Warning, TEXT("Toco alguna cosa"));

		AVRCharacter* Papa = Cast<AVRCharacter>(GetAttachParentActor());
		if (Papa != nullptr)
		{
			APlayerController* PlayControl = Cast<APlayerController>(Papa->GetController());
			if (PlayControl != nullptr)
			{
				PlayControl->PlayHapticEffect(CurveHaptic, MotionController->GetTrackingSource());
			}
		}
	}
	bCanClimb = bNew;
}

void AHandController::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	bCanClimb = CanClimb();
}

bool AHandController::CanClimb() const
{
	TArray <AActor*>AQuiTocu;
	GetOverlappingActors(AQuiTocu);
	for (AActor* Qui : AQuiTocu)
	{
		if (Qui->ActorHasTag(TEXT("Climbable")))
		{
			return true;
		}
	}
	return false;
}

void AHandController::PairController(AHandController* Controller)
{
	OtherController = Controller;
	OtherController->OtherController = this;
}

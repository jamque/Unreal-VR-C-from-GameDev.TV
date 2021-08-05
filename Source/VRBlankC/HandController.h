// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandController.generated.h"

class UMotionControllerComponent;
class USphereComponent;
class UHapticFeedbackEffect_Base;

UCLASS()
class VRBLANKC_API AHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetHand(FName Hand);

	void Grip();
	void Release();
	void PairController(AHandController* Controller);

protected:
	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(EditDefaultsOnly)
	UHapticFeedbackEffect_Base* CurveHaptic;

private:
	// Crides d'esdeveniments - Callback

	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);


	// Helpers
	bool CanClimb() const;

	//State variable
	bool bCanClimb;
	bool bIsClimbing;
	FVector ClimingStartLocation;

	AHandController* OtherController;
};

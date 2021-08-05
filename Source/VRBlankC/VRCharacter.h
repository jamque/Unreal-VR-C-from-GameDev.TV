// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

class UCameraComponent;
class USceneComponent;
class UStaticMeshComponent;
class UPostProcessComponent;
class UMaterialInstanceDynamic;
class UMaterialInterface;
class UCurveFloat;

class USplineComponent;
class USplineMeshComponent;

class AHandController;

UCLASS()
class VRBLANKC_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* VRArrel;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Desti;

	UPROPERTY(VisibleAnywhere)
	UPostProcessComponent* EfecteTunnel;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* TeleportPath;

	UPROPERTY(VisibleAnywhere)
	AHandController* LeftController;

	UPROPERTY(VisibleAnywhere)
	AHandController* RightController;


	UPROPERTY()
	UMaterialInstanceDynamic* VisorMaterialInstancia;

	UPROPERTY(VisibleAnywhere)
	TArray<USplineMeshComponent*> DynamicMeshArray;

	void Camina(float valor);
	void DeCostat(float valor);

		void GripLeft();
		void ReleaseLeft();
		void GripRight();
		void ReleaseRight();

	void MirarAmunt(float valor);
	void GirarCap(float valor);
	float CameraVelocity;

	bool TeTocat;
	FVector nouDesti;
	void IniciaTeleport();
	bool CercaDestiTeleport(TArray<FVector>& Cami, FVector& PuntDesti);
	void DibuixaLaser(const TArray<FVector>& Cami);
	void ActualitzaSpline(const TArray<FVector>& Cami);
	void ActualitzaDestiTeleport();
	void FinalitzaTeleport();

	void ActualitzaVisor();
	FVector2D CalculaCentreVisor();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Teleport")
	float TeleportSpeed;
	//float DistanciaTeleportMax;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Teleport")
	float TempsTeleportFade;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* VisorMaterialBase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCurveFloat* RadiVsVelocitat;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* LineaMesh;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* LineaMaterial;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandController> HandControllerClass;
};

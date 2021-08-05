// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

#include "Camera/CameraComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HandController.h"

/*************/

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/PostProcessComponent.h"

// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRArrel = CreateDefaultSubobject<USceneComponent>(TEXT("VRArrel"));
	VRArrel->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRArrel);

	/*************/

	TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("CamiVisibleTeleport"));
	TeleportPath->SetupAttachment(VRArrel);

	Desti = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CilindreDesti"));
	Desti->SetupAttachment(GetRootComponent());

	EfecteTunnel = CreateDefaultSubobject<UPostProcessComponent>(TEXT("EfecteTunnel"));
	EfecteTunnel->SetupAttachment(GetRootComponent());


	TeTocat = false;
	TeleportSpeed = 800.0f;
	TempsTeleportFade = 0.5f;
	CameraVelocity = 0.7f;

}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	FTransform EmptyTR;
	LeftController = GetWorld()->SpawnActor<AHandController>(HandControllerClass, EmptyTR);
	if (LeftController != nullptr)
	{
		LeftController->AttachToComponent(VRArrel, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		LeftController->SetOwner(this);
		LeftController->SetHand("Left");
	}
	RightController = GetWorld()->SpawnActor<AHandController>(HandControllerClass, EmptyTR);
	if (RightController != nullptr)
	{
		RightController->AttachToComponent(VRArrel, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		RightController->SetOwner(this);
		RightController->SetHand("Right");
	}

	/*************/

	if (VisorMaterialBase != nullptr)
	{
		VisorMaterialInstancia = UMaterialInstanceDynamic::Create(VisorMaterialBase, this);
		EfecteTunnel->AddOrUpdateBlendable(VisorMaterialInstancia);
		VisorMaterialInstancia->SetScalarParameterValue(TEXT("Radius"), 0.6f);

	}
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*************/
	// Launch Raytrace
	ActualitzaDestiTeleport();

	ActualitzaVisor();

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Palante"), this, &AVRPawn::Camina);
	PlayerInputComponent->BindAxis(TEXT("Delao"), this, &AVRPawn::DeCostat);
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Released, this, &AVRPawn::IniciaTeleport);
	PlayerInputComponent->BindAxis(TEXT("MirarAmunt"), this, &AVRPawn::MirarAmunt);
	PlayerInputComponent->BindAxis(TEXT("GirarCap"), this, &AVRPawn::GirarCap);

}

void AVRPawn::Camina(float valor)
{
	AddMovementInput(Camera->GetForwardVector(), valor);
}

void AVRPawn::DeCostat(float valor)
{
	AddMovementInput(Camera->GetRightVector(), valor);
}

void AVRPawn::MirarAmunt(float valor)
{
	//AddControllerPitchInput( - 100.0f * valor * GetWorld()->GetDeltaSeconds());
}

void AVRPawn::GirarCap(float valor)
{
	//AddControllerYawInput(100.0f * valor * GetWorld()->GetDeltaSeconds());
}

void AVRPawn::IniciaTeleport()
{
	if (TeTocat)
	{
		//Calcul del Desti, AL MOMENT de fer Click
		//UCapsuleComponent* Cap = GetCapsuleComponent();
		//float radi = Cap->GetScaledCapsuleRadius();
		//FVector alt = Cap->GetScaledCapsuleHalfHeight() * GetActorUpVector();
		float radi = 34.0f;
		FVector alt = 88.0f * GetActorUpVector();
		nouDesti = Desti->GetComponentLocation() + alt - LeftController->GetActorForwardVector() * radi;

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC != nullptr) // Fer sempre que hi hagi un Cast
		{
			PC->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, TempsTeleportFade, FLinearColor::Black, false, true);
		}
		FTimerHandle timerH;
		GetWorldTimerManager().SetTimer(timerH, this, &AVRPawn::FinalitzaTeleport, TempsTeleportFade);
	}
}

bool AVRPawn::CercaDestiTeleport(TArray<FVector>& Cami, FVector& PuntDesti)
{
	bool resultat = false;
	//FHitResult QuiHaSigut;
	FVector Inici = LeftController->GetActorLocation();
	FVector Fi = LeftController->GetActorForwardVector();
	//Fi = Fi.RotateAngleAxis(30, LeftController->GetRightVector());
	Fi *= TeleportSpeed;

	FPredictProjectilePathParams ParametresParabola;
	//FPredictProjectilePathParams Params(
	//	10.0f,
	//	Inici,
	//	Fi,
	//	1.0f,
	//	ECollisionChannel::ECC_Visibility,
	//	this
	//);
	ParametresParabola.ProjectileRadius = 10.0f;
	ParametresParabola.StartLocation = Inici;
	ParametresParabola.LaunchVelocity = Fi;
	ParametresParabola.MaxSimTime = 1.0f;
	ParametresParabola.bTraceWithCollision = true;
	ParametresParabola.TraceChannel = ECollisionChannel::ECC_Visibility;
	ParametresParabola.ActorsToIgnore.Add(this);
	//ParametresParabola.DrawDebugType = EDrawDebugTrace::ForOneFrame;

	FPredictProjectilePathResult ResultatParabola;
	TeTocat = UGameplayStatics::PredictProjectilePath(this, ParametresParabola, ResultatParabola);

	if (!TeTocat) return resultat;

	for (FPredictProjectilePathPointData punt : ResultatParabola.PathData)
	{
		Cami.Add(punt.Location);
	}

	FVector Onhetocat = ResultatParabola.HitResult.Location;

	FNavLocation Projecciotocat;
	UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld());
	if (Nav != nullptr)
	{
		if (Nav->ProjectPointToNavigation(Onhetocat, Projecciotocat, FVector(100, 100, 100)))
		{
			PuntDesti = Projecciotocat.Location;
			resultat = true;
		}
	}

	return resultat;
}

void AVRPawn::DibuixaLaser(const TArray<FVector>& Cami)
{
	ActualitzaSpline(Cami);

	for (USplineMeshComponent* Segment : DynamicMeshArray)
	{
		Segment->SetVisibility(false);
	}
	int32 NSegments = Cami.Num() - 1;
	for (int32 i = 0; i < NSegments; i++)
	{
		// Si calen més punts en crea de nous.
		if (DynamicMeshArray.Num() <= i)
		{
			USplineMeshComponent* DynamicMesh = NewObject<USplineMeshComponent>(this);
			DynamicMesh->SetMobility(EComponentMobility::Movable);
			DynamicMesh->AttachToComponent(TeleportPath, FAttachmentTransformRules::KeepRelativeTransform);
			DynamicMesh->SetStaticMesh(LineaMesh);
			DynamicMesh->SetMaterial(0, LineaMaterial);
			DynamicMesh->RegisterComponent();
			DynamicMeshArray.Add(DynamicMesh);
		}
		//Seteja el punt a visible i li dona forma
		DynamicMeshArray[i]->SetVisibility(true);
		FVector Posicio, Tangent;
		FVector PosicioFi, TangentFi;
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i, Posicio, Tangent);
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i + 1, PosicioFi, TangentFi);
		DynamicMeshArray[i]->SetStartAndEnd(Posicio, Tangent, PosicioFi, TangentFi);
	}
}

void AVRPawn::ActualitzaSpline(const TArray<FVector>& Cami)
{
	TeleportPath->ClearSplinePoints(false);
	for (int32 i = 0; i < Cami.Num(); i++)
	{
		FVector posicio = TeleportPath->GetComponentTransform().InverseTransformPosition(Cami[i]);
		FSplinePoint puntCurva(i, posicio, ESplinePointType::Curve);
		TeleportPath->AddPoint(puntCurva, false);
	}
	TeleportPath->UpdateSpline();
}

void AVRPawn::ActualitzaDestiTeleport()
{
	TArray<FVector> Path;
	FVector PosicioTeleport;
	if (CercaDestiTeleport(Path, PosicioTeleport))
	{
		Desti->SetVisibility(true);
		Desti->SetWorldLocation(PosicioTeleport);
		DibuixaLaser(Path);
	}
	else
	{
		Desti->SetVisibility(false);
		TArray<FVector> PathBuit;
		DibuixaLaser(PathBuit);
	}
}

void AVRPawn::FinalitzaTeleport()
{
	SetActorLocation(nouDesti);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC != nullptr) // Fer sempre que hi hagi un Cast
	{
		PC->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, TempsTeleportFade, FLinearColor::Black, false, true);
	}
}

void AVRPawn::ActualitzaVisor()
{
	float Velocitat = GetVelocity().Size();
	if ((RadiVsVelocitat != nullptr) && (VisorMaterialInstancia != nullptr))
	{
		float Radi = RadiVsVelocitat->GetFloatValue(Velocitat);
		VisorMaterialInstancia->SetScalarParameterValue(TEXT("Radius"), Radi);

		FVector2D centre = CalculaCentreVisor();
		FLinearColor dada = FLinearColor(centre.X, centre.Y, 0.0f);
		VisorMaterialInstancia->SetVectorParameterValue(TEXT("Centre"), dada);
	}
}

FVector2D AVRPawn::CalculaCentreVisor()
{
	FVector2D posicioScreen = FVector2D(0.5f, 0.5f);
	FVector direccio = GetVelocity().GetSafeNormal();
	if (direccio.IsNearlyZero())
	{
		return posicioScreen;
	}
	FVector posicioWorld = Camera->GetComponentLocation();
	float arcos = FVector::DotProduct(Camera->GetForwardVector().GetSafeNormal(), direccio);
	if (arcos < 0.05f)
	{
		if (arcos > 0.0f)
		{
			posicioWorld += direccio * 1000.0f;
		}
		else
		{
			posicioWorld -= direccio * 1000.0f;
		}
	}
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC != nullptr) // Fer sempre que hi hagi un Cast
	{
		if (PC->ProjectWorldLocationToScreen(posicioWorld, posicioScreen))
		{
			int32 W, H;
			PC->GetViewportSize(W, H);
			posicioScreen.X /= W;
			posicioScreen.Y /= H;
		}
	}


	return posicioScreen;
}

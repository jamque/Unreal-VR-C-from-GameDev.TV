// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"
//#include "DrawDebugHelpers.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

#include "HandController.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRArrel = CreateDefaultSubobject<USceneComponent>(TEXT("VRArrel"));
	VRArrel->SetupAttachment(GetRootComponent());

	// In Oculus Quest Tracking Origin is the floor, then Move HDR to Capsule Floor.
	FVector OculusOriginPositionCorrection(0.0f, 0.0f, - GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	VRArrel->AddRelativeLocation(OculusOriginPositionCorrection);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRArrel);

	TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("CamiVisibleTeleport"));
	TeleportPath->SetupAttachment(VRArrel);

	Desti =  CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CilindreDesti"));
	Desti->SetupAttachment(GetRootComponent());

	EfecteTunnel = CreateDefaultSubobject<UPostProcessComponent>(TEXT("EfecteTunnel"));
	EfecteTunnel->SetupAttachment(GetRootComponent());


	TeTocat = false;
	TeleportSpeed = 800.0f;
	TempsTeleportFade = 0.5f;
	CameraVelocity = 0.7f;
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);


	if (VisorMaterialBase != nullptr)
	{
		VisorMaterialInstancia = UMaterialInstanceDynamic::Create(VisorMaterialBase, this);
		EfecteTunnel->AddOrUpdateBlendable(VisorMaterialInstancia);
		VisorMaterialInstancia->SetScalarParameterValue(TEXT("Radius"), 0.6f);

	}
	FTransform EmptyTR;
	LeftController = GetWorld()->SpawnActor<AHandController>(HandControllerClass, EmptyTR);
	if (LeftController != nullptr)
	{
		LeftController->AttachToComponent(VRArrel,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		LeftController->SetOwner(this);
		LeftController->SetHand(TEXT("Left"));
	}
	RightController = GetWorld()->SpawnActor<AHandController>(HandControllerClass, EmptyTR);
	if (RightController != nullptr)
	{
		RightController->AttachToComponent(VRArrel, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		RightController->SetOwner(this);
		RightController->SetHand(TEXT("Right"));
	}
	LeftController->PairController(RightController);
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector relPos = Camera->GetComponentLocation() - GetActorLocation();
	relPos.Z = 0.0f; // Only if always walk on a perpendicular plane.
	// General case: FVector::VectorPlaneProject();
	AddActorWorldOffset(relPos);
	VRArrel->AddWorldOffset(-relPos);


	// Launch Raytrace
	ActualitzaDestiTeleport();

	ActualitzaVisor();
}
void AVRCharacter::ActualitzaVisor()
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

FVector2D AVRCharacter::CalculaCentreVisor()
{
	FVector2D posicioScreen = FVector2D(0.5f, 0.5f);
	return posicioScreen;
	FVector direccio = GetVelocity().GetSafeNormal();
	if (direccio.IsNearlyZero()) 
	{
		return posicioScreen;
	}
	FVector posicioWorld = Camera->GetComponentLocation();
	float arcos = FVector::DotProduct(Camera->GetForwardVector().GetSafeNormal(), direccio);
	if ( arcos < 0.05f)
	{
		if ( arcos > 0.0f)
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

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Palante"), this, &AVRCharacter::Camina);
	PlayerInputComponent->BindAxis(TEXT("Delao"), this, &AVRCharacter::DeCostat);
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Released, this, &AVRCharacter::IniciaTeleport);
	PlayerInputComponent->BindAxis(TEXT("MirarAmunt"), this, &AVRCharacter::MirarAmunt);
	PlayerInputComponent->BindAxis(TEXT("GirarCap"), this, &AVRCharacter::GirarCap);

	PlayerInputComponent->BindAction(TEXT("GripLeft"), IE_Pressed, this, &AVRCharacter::GripLeft);
	PlayerInputComponent->BindAction(TEXT("GripLeft"), IE_Released, this, &AVRCharacter::ReleaseLeft);
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Pressed, this, &AVRCharacter::GripRight);
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Released, this, &AVRCharacter::ReleaseRight);
}

void AVRCharacter::Camina(float valor)
{
	AddMovementInput(Camera->GetForwardVector(), valor);
}

void AVRCharacter::DeCostat(float valor)
{
	AddMovementInput(Camera->GetRightVector(), valor);
}

void AVRCharacter::GripLeft()
{
	LeftController->Grip();
}

void AVRCharacter::ReleaseLeft()
{
	LeftController->Release();
}

void AVRCharacter::GripRight()
{
	RightController->Grip();
}

void AVRCharacter::ReleaseRight()
{
	RightController->Release();
}

void AVRCharacter::MirarAmunt(float valor)
{
	AddControllerPitchInput( - 100.0f * valor * GetWorld()->GetDeltaSeconds());
}

void AVRCharacter::GirarCap(float valor)
{
	AddControllerYawInput(100.0f * valor * GetWorld()->GetDeltaSeconds());
}

void AVRCharacter::DibuixaLaser(const TArray<FVector>& Cami)
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

void AVRCharacter::ActualitzaSpline(const TArray<FVector>& Cami)
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

void AVRCharacter::IniciaTeleport()
{
	if (TeTocat)
	{
		//Calcul del Desti, AL MOMENT de fer Click
		UCapsuleComponent* Cap = GetCapsuleComponent();
		float radi = Cap->GetScaledCapsuleRadius();
		FVector alt = Cap->GetScaledCapsuleHalfHeight() * GetActorUpVector();
		nouDesti = Desti->GetComponentLocation()+ alt - LeftController->GetActorForwardVector() * radi;

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC != nullptr) // Fer sempre que hi hagi un Cast
		{
			PC->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, TempsTeleportFade, FLinearColor::Black, false, true);
		}
		FTimerHandle timerH;
		GetWorldTimerManager().SetTimer(timerH, this, &AVRCharacter::FinalitzaTeleport, TempsTeleportFade);
	}
}

bool AVRCharacter::CercaDestiTeleport(TArray<FVector> &Cami, FVector& PuntDesti)
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

void AVRCharacter::ActualitzaDestiTeleport()
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

void AVRCharacter::FinalitzaTeleport()
{
	SetActorLocation(nouDesti);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC != nullptr) // Fer sempre que hi hagi un Cast
	{
		PC->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, TempsTeleportFade, FLinearColor::Black, false, true);
	}
}


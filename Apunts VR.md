# Setup VR for develop in Oculus Quest
*With a little help from Quinn Kuslich https://www.youtube.com/watch?v=HgwXqRuEOo0*

Hardware:
- Oculus Quest HMD
- USB type C cable (charging cable)
- Bluetooth in Pc Computer

Software:
- Unreal Engine
- Android Studio

## Step 1 - Creating an Oculus Developer Organization
- Create an Oculus Creator Account. https://developer.oculus.com/.
- Login with your facebook account.
- Create an Oculus developer organization.

## Step 2 - Placing the Oculus Quest into Developer Mode
Put your Oculus Quest into developer mode, using your mobile Oculus app.(Maybe you need to restart).
## Step 3 - Setting up Android Studio
- Check https://docs.unrealengine.com/4.26/en-US/SharingAndReleasing/Mobile/Android/AndroidSDKRequirements/ to choose a correct version for Android Studio for you Unreal Engine version. **[AND NEVER UPDATE]**
- For example: Unreal 4.26 version: NDK Version : NDK r21b and Android Studio Version 4.0
- Download Android Studio (from donwload archive https://developer.android.com/studio/archive) for windows and install it in your computer.
- After install, open it and Choose "Do not import settings". Also choose not to share your data with Google.
- Setup Android Studio:
  - Choose Custom
  - Select your file location for Gradle (a build automation tool for multi-language software development). Leave as default.
  - Select your UI Theme (dark/light)
  - SDK Components Setup by default too.
  - Emulator Setting by default too.
  - Click finish and wait for install process (may take a while).
  - *Uninstall SDK tools 31.0. Unreal will download itself the version that need when compile the first time.*
  - Close Android Studio.

## Step 4 - Setting  up the Oculus Developer Hub
- Download Oculus Developer Hub Aplication from https://developer.oculus.com/ and install it on your computer.
- Login with your facebook account.
- Open program.
  - Maybe you will be answered about ADB (Android Debug Bridge) client and version that it use. If this happens press Change ADB path. This actions need to restart Oculus Developer Hub.
  - Make sure that your Org Manager tab are using your Name organization setted in step 1.
  - BEWARE! Pc must have Bluetooth to link with Oculus Quest!
  - If you connect your headset, click on My device to manage it. If it doesn't check inside your headset and allow your machine to access your headsets data.

## Step 5 - Setting up Unreal Engine for Oculus Quest Development
- Open up the file explorer and locate where you have installed your Unreal Engine version.
- Go to Engine/Extras/Android
- Locate a file called SetupAndroid.Bat and run it.
  - If you have an Update failed message be sure that you installed a correct version for Android Studio for you Unreal Engine version. Check **Step 3**
 
- Accept license Agreement (y) and a download starts
- Press any key after the process to close process.

**That all. You have your PC, headset and Unreal engine for Quest development.**

## Step 6 - Creating our Unreal Project and Packaging to the Oculus Quest 2
- Make projects with:
   - Target hardware: **Mobile/tablet**
   - Performace settings to **scalable 3D or 2D**
   *You can change in a opened project in Edit Menu ->Project setting Target Hardware*
   
- In the Level map (for a best performace):
   - Use *directtional lights* with *Mobility* to *Static*
   - Put *Intensity* to *1.15 lux* (this is ad advice only)

- In *Edit Menu ->Plugins*:
   - Locate Virtual tab ann make sure that the Oculur VR plugin is enable.
   
- In *Edit Menu ->Project setting*:
   - Make sure that your Game Default is the map you want. 
   - In *Supported Platforms* make check the android box and the windows (32-bit) box
   - In *Engine ->rendering* 
     - Set *Mobile MSAA* to 4x MSAA
	 - Set *VR ->Mobile HDR* to check.
   - In *Platforms ->Android*
     - At the top of the tab you should see a red banner. Click in *Configure Now*
     - In *Android Package name* put a name to your aplication with this formula. If you don't the packaging process will fail. ***com.company.gamename***.
	 - In *Minimum SDK Version* set to 23
	 - In *Target SDK Version* set to 25
	 - Set *Enable FullScreen Immersive on KitKat and abobe devices* to check.
	 - In *Advanced APK Packaging section ->* set *Remove Oculus Signature Files from Distribution APK*.
	 - Add *Package for Oculus Mobile devices* and set *Oculus Quest* or *Oculus Quest2*
- In Editor *Launch* Icon, now you can choose your device where you want to launch the game, **Oculus Quest 2** in that case. This launch the packaging process.

**BEWARE! You maybe see that Unreal Editor looks frozen. NO. DO NOT STOP Unreal task. WAIT. It have a lot of work to do. 3500 shaders, for example.**

**BEWARE! If project fails to build. Go to Android Studio and Uninstall SDK tools 31.0. Unreal will download itself the version that need.**

- In Output Log, when process finish, you would see a line like *Stating Intent* followed by your poject name. This marks that your game is running in your Oculus headset.

## Step 7 - Testing our Oculus Quest Build
Launch Icon in Unreal runs your game directly in your Oculus Quest 2 headset. But you can found your APK in Apps/Unknown Sources folder in Oculus Menu and execute it.
## Step 8 - Sharing our test build with others
Before create a .APK:
- In Unreal Editor in *Edit ->Project Settings* search in *Platforms-> Android* tab and check *Package game data inside .APK?*

In Unreal Editor Select Menu *File->PackageProject->Android->Android ASTC*, select folder destination where you want your .APK file. And the packaging process will begin.

In the process, Unreal create a folder named **Android_ASTC** in the destination folder selected with these files:
- Uninstall_[ProjectName]-armv7.bat
- Install_[ProjectName]-armv7.bat
- [ProjectName]-armv7.apk

Send this .APK file to your friends

Use SideQuest https://sidequestvr.com/ to upload your game in another Oculus headset.

---
## Some notes and possible errors:

UE4 Build Error When building project for Android.
```
    LogPlayLevel: Error: ERROR: System.ArgumentException: Attempt to construct staged filesystem reference from absolute path (/Content/StarterContent/Textures/SomeTexture.uasset). Staged paths are always relative to the staging root.
```
To fix, open Command Prompt and type:
```
C:\Users\adamos>adb shell
hero2lte:/ $ cd sdcard
hero2lte:/sdcard $ ls
Alarms DCIM GSWave Music Pictures Podcasts Samsung
Android Download Movies Notifications Playlists Ringtones UE4Game
hero2lte:/sdcard $ rm -r UE4Game/
```
---
---
---

# Step tp VR

Unreal detects automaticaly translation and rotation for headset and put values in camera character.

1-Movement
Us a character for VR movement. Because is a human like and match well with our movements.
- Create a new class from Character. VRCharacter
- Must make your own gamemode BP from GameModeBase
   - Set default pawnclass as VRCharacter
- Insert a new component in VRCharacter. UCameraComponent pointer as private named  Camera.
- Attach to root component in VRCharacter constructor
```c
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
```
	
- Create axis (There are inputs specifics for VR Oculus inputs touch)
- bind them
- Use AddMovementInput in bind functions to move character where Camera look forward and strafe

---

El mandos van sols
- Oculus touch Thumbstick X 
- Oculus touch Thumbstick Y

Els poses a input i au.

---

Move cam component and Actor at the same place even you move in space.
Playspace Offset
If we move root capsule, head display move to. We must to reposition head display.

- Create a USceneComponent and use as root (VRRoot). This is child of RootComponent (Capsule in this case)
- Setup Camera as root of this USceneComponent.
```c
	FVector relPos = Camera->GetComponentLocation() - GetActorLocation();
	relPos.Z = 0.0f; // Only if always walk on a perpendicular plane.
	// General case: FVector::VectorPlaneProject();
	AddActorWorldOffset(relPos);
	VRArrel->AddWorldOffset(-relPos);
```  
---
## VR Sickness
See movement when you are not moving
Senses: (3)
- Visual (reference point)
- Vestibular system (inner ear - cochlea)
- Propriosensory system (musucar position sensor - nerves) 6th sense.

In VR we make these systems in contradiction.
Your body to Agree with all systems:
1-ignore one system or part of it (see thw horitzont)
2-suppress system (close eyes, to black)

How can We avoid VR sickness?
- Diana, one point without movement
- Helmet or cockpit, border without movement
- Teleport?
- Tunnel or blinkers, fade to tunnel

| Technique | Visual | Vestibular | Propriosensoty | Cons? |
|---|---|---|---|---|
| Movement in play space | Fooled by tracking head closesy | True movement | True movement | Limited by play space size |
| Teleporting | Suppressed by fading in/out during teleport | No movement | No movement | Not very immersive. (Implemented: error to teleport up-dow or long walks to arrive to point. |
| Constant/slow acceleration | Ignore | Minimal diference sensed | Ignore | Limits agility.|
| Climbing | Point of reference (hand) | Ignored | Simulñated by hand movement | Only good for climbing games |
| Joggins on the spot | Ignored. | Jogging motion confuses the system | Jogging motion confuses the system | Awkward for the player with poor detection |
| Cockpit simulation | Fixed reference and minimized peripheral vision | Ignored | Sitting simulation anyway | Still can't hadle very jerky motion. |
| Blinkers | Minimized peripheral vision | Movements always in direction of head | Ignored | Limits how movement can work |

---
## Teleport
How Teleport Will work
1- Find the teleport destination
2- Show the destination to the player (cylinder with material)
3- Player clicks and FAde out the viewport
4- Move the player to the destination
5- Fade back in

1- 
Add a StaticMesh Component to player and setup attachment to RootComponent.
Add cylinder Mesh, via BP. Make a BP son of you BPCharacter.

Create a function `UpdateDestination`.
Create and Use a `DistanceTeleport` class attribute to make distance to teleport parametrical.

Tracing LineTraceSingleByChannel
ECC_Visibility
DrawDebugLine

Use of `GetWorld`, as a global pointer for utilities.

```c
	FHitResult QuiHaSigut;
	FVector Inici = Camera->GetComponentLocation();
	FVector Fi = Camera->GetForwardVector() * DistanciaTeleportMax;
	bool TeTocat = GetWorld()->LineTraceSingleByChannel(QuiHaSigut, Inici, Fi, ECC_Visibility);

	if (TeTocat)
	{
		DrawDebugLine(GetWorld(), Inici, QuiHaSigut.Location, FColor::Red);
		Desti->SetVisibility(true);
		Desti->SetWorldLocation(QuiHaSigut.Location);
	}
	else
	{
		Desti->SetVisibility(false);
		DrawDebugLine(GetWorld(), Inici, Fi, FColor::Blue);
	}
```

---
## Materials
cylinder collision with himself, at next frame.
Change to No collision preset
If you create a VR BP Project....wait...39%...wait shaders compialtion in CPU.
After that 5600 shader start to compile.
`Migrate -> MI_TeleportCylinderPreview`
---
Fades, timers and teleporting
Fade in and out timing must change by designers

3- Player clicks and Fade out the viewport
4- Move the player to the destination
5- Fade back in

3-
Assign in input project settings, action button to teleport.
BindAction in Character with IE_Released
https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Engine/EInputEvent/
4-
Move Actor to Actor Component Location
	`SetActorLocation(Desti->GetComponentLocation());`
	
5-
GetWorldTimerManager
FTimer::ManagerSetTimer
APlayerController
	APlayerCameraManager::StarCameraFade

- Read the linked APIs
https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/GameFramework/AActor/GetWorldTimerManager/
https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/GameFramework/APlayerController/PlayerCameraManager/
https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Camera/APlayerCameraManager/StartCameraFade/
https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/FTimerManager/SetTimer/5/
https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Components/UCapsuleComponent/
    GetScaledCapsuleRadius()

- Fade the camera out
- When that's finished: Teleport
- Then fade in again (configurable by designer)
- Debug the teleport issue
	- Where is the actor center?

```c
#include "Components/CapsuleComponent.h"

void AVRCharacter::IniciaTeleport()
{
	if (TeTocat)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC != nullptr) // Fer sempre que hi hagi un Cast
		{
			PC->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, TempsTeleportFade, FLinearColor::Black, false, true);
		}
		FTimerHandle timerH;
		GetWorldTimerManager().SetTimer(timerH, this, &AVRCharacter::FinalitzaTeleport, TempsTeleportFade);
	}
}

void AVRCharacter::FinalitzaTeleport()
{
	FVector nouDesti;
	UCapsuleComponent *Cap = GetCapsuleComponent();
	float radi = Cap->GetScaledCapsuleRadius();
	float alt = Cap->GetScaledCapsuleHalfHeight();
	nouDesti = Desti->GetComponentLocation() - Camera->GetForwardVector() * radi;
	nouDesti.Z += alt;
	SetActorLocation(nouDesti);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC != nullptr) // Fer sempre que hi hagi un Cast
	{
		PC->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, TempsTeleportFade, FLinearColor::Black, false, true);
	}
}
```

---
## Projecting Onto the NavMesh
Teleporting where I can be, not in walls or air places. Using navmesh.
Maybe using a surface normal it could be enough, but it is not enought.

Use Nav Mesch Bounds Volumen and extend to your level. 

You must to project your destination point to Nav Mesh
using UNavigationSystem::ProjectPointToNavigation
UNavigationSystem is in the World.

UNavigationSystemV1::GetCurrent

Try yourself:
- Using API information
- Project the hit location
- What happens if there's no projection?
- and Test.
- Refactor (becaus in border you have some new problem)

ProjectPointToNavigation returns an FNavLocation. It's an struct, like a raycast result with a lot of data inside.
We want the location of this struct.

In file projectname.Build.cs add NavigationSystem Module
   PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem" });

```c
#include "NavigationSystem.h"

In ActualitzaDestiTeleport

	if (TeTocat)
	{
		FVector Onhetocat = QuiHaSigut.Location;
		FNavLocation Projecciotocat;
		UNavigationSystemV1 *Nav = UNavigationSystemV1::GetCurrent(GetWorld());
		if (Nav != nullptr)
		{
			if (Nav->ProjectPointToNavigation(Onhetocat, Projecciotocat))
			{
				DrawDebugLine(GetWorld(), Inici, Projecciotocat.Location, FColor::Red);
				Desti->SetVisibility(true);
				Desti->SetWorldLocation(Projecciotocat.Location);
			}
		}
	}
```

Refactoring two functions

```c
bool AVRCharacter::CercaDestiTeleport(FVector& PuntDesti)
{
	bool resultat = false;
	FHitResult QuiHaSigut;
	FVector Inici = Camera->GetComponentLocation();
	FVector Fi = Camera->GetForwardVector() * DistanciaTeleportMax;
	TeTocat = GetWorld()->LineTraceSingleByChannel(QuiHaSigut, Inici, Fi, ECC_Visibility);
	if (TeTocat)
	{
		FVector Onhetocat = QuiHaSigut.Location;
		FNavLocation Projecciotocat;
		UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld());
		if (Nav != nullptr)
		{
			if (Nav->ProjectPointToNavigation(Onhetocat, Projecciotocat))
			{
				PuntDesti = Projecciotocat.Location;
				resultat = true;
			}
		}
	}
	return resultat;
}

void AVRCharacter::ActualitzaDestiTeleport()
{
	FVector PosicioTeleport;
	if (CercaDestiTeleport(PosicioTeleport))
	{
		Desti->SetVisibility(true);
		Desti->SetWorldLocation(PosicioTeleport);
	}
	else
	{
		Desti->SetVisibility(false);
	}
}
```
---
Create blinkness in viewport, like a helmet
with post processing.
Prevent VR sickness when we run or move fast. Because this add a reference point
to look.
Use a Material in a Post processing Volumen.
Use Infinite Extent to use this Post processing Volume in the whole world.
In this material, change Material Domain to Post Process
and Add a Constant Vector 4, to give a solid color.
We go to add a in Post Process Materials a new material from Asset.
Material Nodes: 
 - SceneTexture:PostProcessInput0
 - ScreenPosition
 - RadialGradientExponential
 - Multiply

Create a Tunnel Effect
- Play with Nodes
- Remember to preview their results in game
- Can you make a tunnel effect Material?

---
Use Dynamic Material Instance 
to change Radius for tunnel dynamicaly

Create a PostProcessComponent in VRCharacter
and a Material Interface reference in VRCharacter in order to assign
a material BASE for Tunnel vision

- Remove post processing from the level
- Create a dynamic material instance `UMaterialInstanceDynamic::Create` reference from material Base	Intance private and Base protected and Bluprint editable.
- Configure the post processing component `EfecteTunnel->AddOrUpdateBlendable(VisorMaterialInstancia);`
\[ALERT\]: When you recompile C Class, BP Child Class,
  LOST Material Base Reference, or not] (Random editor lose things)
- Set the radius in code but only in general mode. `BeginPlay`
	
```c
	if (VisorMaterialBase != nullptr)
	{
		VisorMaterialInstancia = UMaterialInstanceDynamic::Create(VisorMaterialBase, this);
		EfecteTunnel->AddOrUpdateBlendable(VisorMaterialInstancia);
		VisorMaterialInstancia->SetScalarParameterValue(TEXT("Radius"), 0.6f);

	}
```

---
Adjusting Radius with a Curve
When your velocity change, Tunnel vision grows

Set Axis for hand controllers in Projects settings -> Input
Add  controllers (thumbstick) for each hand controler. In steamVR names must
be `Move_X Move_Y` for Axis.

Add new `Miscellaneus->Curve` to Content Folder. Curve float.

Curves have GetFloatValue to get Value.
Add UCurveFloat* to Character in C++ in order to give our asset curve via BP

- Setup Curve
- Calculate the speeed
- Get the radius from curve
- Set the radius to material instance


```c
void AVRCharacter::ActualitzaVisor()
{
	float Velocitat = GetVelocity().Size();
	if ((RadiVsVelocitat != nullptr) && (VisorMaterialInstancia != nullptr))
	{
		float Radi = RadiVsVelocitat->GetFloatValue(Velocitat);
		VisorMaterialInstancia->SetScalarParameterValue(TEXT("Radius"), Radi);
	}
}
```

---
Creating tunnel vision.
Visor in walk direction.

Where is the center of motion?
It's not in center of screen.
(Hi ha una explained gràfic interesant al video)
Project the velocity vector on screen. We can found the center where to draw 
our Visor.
Calculating The center
1- get direction of movement
2- Where does it point in the world?
3- Project that to screen
4- Convert to UV coordinates.

Modify Material.
RadioGradientExponential -> Parameter Center Position must be variable, as a parameter 

Create a function that returns a FVector2D to update this new material parameter in Material.

To projectScreen
APlayerController::ProjectWorldLocationToScreen
Use Velocity as Normal with .GetSafeNormal()

Use `IsNearlyZero()` to check `FVector == 0.0f`

When Calculate WorldLocation *100 or *1000 MoveVector

APlayerController::GetViewportSize use to calculate position screen in 1.0 factor.

Calculate The Center
- 
1- get direction of movement
2- Where does it point in the world?
3- Project that to screen
4- Convert to UV coordinates.
- The focus of the circle should be still
- BONUS: Make going backwards work

```c
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
```

```c
FVector2D AVRCharacter::CalculaCentreVisor()
{
	FVector2D posicioScreen = FVector2D(0.5f, 0.5f);
	FVector direccio = GetVelocity().GetSafeNormal();
	if (direccio.IsNearlyZero()) 
	{
		return posicioScreen;
	}
	FVector posicioWorld;
	if (FVector::DotProduct(Camera->GetForwardVector().GetSafeNormal(), direccio) > 0.0f)
	{
		posicioWorld = Camera->GetComponentLocation() + direccio * 1000.0f;
	}
	else
	{
		posicioWorld = Camera->GetComponentLocation() - direccio * 1000.0f;
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
```

:warning: **ALERTA BUG !!!**

Al Provar en VR. Que calculi primer on va, que inici el Fade i abans de tornar es quan posiciona.

------------------------------
# FI PART 1
------------------------------

Hand Controller Components
Mark teleport destination with Controllers.
UMotionControllerComponent
Has a transform and tracks controllers
Need to tell with hand is. Then we need 2.

We need to include a new module in Name.Builds.cs
"HeadMountedDisplay"

Attach to VRRoot / VRArrel
and SetTrackingSource for left/right

Set a Mesh for each hand
You can Use defautl Engine Content

- Give them a Mesh
bDisplayDeviceModel = true;

- Use controllers instead of head position
In IniciaTeleport and CercaDestiTeleport functions change camera references for LeftController
- Can you make it more comfortable?
Rotate in Right Axis, LeftController Forward vector in CercaDestiTeleport in order to use controller a little bit rotated to point to gorund conformtable.
Use RotateAngleAxis in FVector.

---
Parabolic Teleport Pointer
Draw a line from Controller to Teleport Point
Use a Parabolic path. We must to Draw a Parabolic Path
USpline
Deforming mesh sections
1- Predict the projectil path
  UGameplayStatics::PredictProjectilePath
2- Create a spline interpolation
  USplineComponent
3- Overlay with segments fo deformed mesh
  USplineMeshComponent 

Predict the Parabola
- Construct the parameters
- Setup Debug tracing
- Set the teleport location from the parabola
- Play test

```c
#include "Kismet/GameplayStatics.h"
UGameplayStatics::PredictProjectilePath

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

	ParametresParabola.DrawDebugType = EDrawDebugTrace::ForOneFrame;


	FPredictProjectilePathResult ResultatParabola;
	TeTocat = UGameplayStatics::PredictProjectilePath(this, ParametresParabola, ResultatParabola);
```

---
Using A USplineComponent
To draw 
2- Create a spline interpolation
USplineComponent is a USceneComponent

Add a new component to VRCharacter USplineComponent type

Then BP adquire this component, where you can add points manually.


Update the Spline
- Use the path from the results
- Read the splines docs
- Add a point per path point
- What type of point should they be?

AddPoints from USplineComponent

```c
FSplinePoint
// Inside if (Nav->ProjectPointToNavigation(Onhetocat, Projecciotocat))
//   at AVRCharacter::CercaDestiTeleport(FVector& PuntDesti)
	// Preparo el array de punts de la curva a dibuixar
	TArray<FSplinePoint> moltsPunts;
	float ID = 0.0f;
	for (FPredictProjectilePathPointData punt : ResultatParabola.PathData)
	{
		// Cal passar els valors absoluts del resultat a valors relatius a on es
		//   l'origen del teleport.
		FVector posicio = TeleportPath->GetComponentTransform().InverseTransformPosition(punt.Location);
		FSplinePoint puntCurva(ID, posicio, ESplinePointType::Curve);
		moltsPunts.Add(puntCurva);
		ID++;
	}
	TeleportPath->ClearSplinePoints(false);
	TeleportPath->AddPoints(moltsPunts);
```

---
## Dynamically Constructing UObjects

Draw Sphere dinamically where there are points from the Splines.

3- Overlay with segments fo deformed mesh
  USplineMeshComponent Array

UObject Instance creation NewObject template
Create a UStaticMeshComponent using NewObject and AttachToComponent in BeginPlay.
And don't forget to RegisterComponent

SetStaticMesh and SetMaterial before RegisterComponent
and create 
UStaticMesh and UMaterialInterface with UPROPERTY(EditDefaultsOnly)
This can be defined in BP.

Create an Object Pool
- For each element in the path
- Create a static mesh object (for each point) (NO in any single frame)
- Store in an array
- Only create new if needed.

---
## Deforming Meshes With Splines

Do not Remove meshes from array. Use SetVisibility and always create meshes.

```c
bool AVRCharacter::CercaDestiTeleport(FVector& PuntDesti)
{
	bool resultat = false;
	//FHitResult QuiHaSigut;
	FVector Inici = LeftController->GetComponentLocation();
	FVector Fi = LeftController->GetForwardVector();
	//Fi = Fi.RotateAngleAxis(30, LeftController->GetRightVector());
	Fi *= TeleportSpeed;
	//Fi *= DistanciaTeleportMax;

	FPredictProjectilePathParams ParametresParabola;
	ParametresParabola.ProjectileRadius = 10.0f;
	ParametresParabola.StartLocation = Inici;
	ParametresParabola.LaunchVelocity = Fi;
	ParametresParabola.MaxSimTime = 1.0f;
	ParametresParabola.bTraceWithCollision = true;
	ParametresParabola.TraceChannel = ECollisionChannel::ECC_Visibility;
	ParametresParabola.ActorsToIgnore.Add(this);

	ParametresParabola.DrawDebugType = EDrawDebugTrace::ForOneFrame;


	FPredictProjectilePathResult ResultatParabola;
	TeTocat = UGameplayStatics::PredictProjectilePath(this, ParametresParabola, ResultatParabola);
	//TeTocat = GetWorld()->LineTraceSingleByChannel(QuiHaSigut, Inici, Fi, ECC_Visibility);
	//GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Blue, FString::FromInt(TeTocat2));
	//GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Orange, FString::FromInt(TeTocat));
	if (TeTocat)
	{
		//FVector Onhetocat = QuiHaSigut.Location;
		FVector Onhetocat = ResultatParabola.HitResult.Location;//QuiHaSigut.Location;

		FNavLocation Projecciotocat;
		UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld());
		if (Nav != nullptr)
		{
			if (Nav->ProjectPointToNavigation(Onhetocat, Projecciotocat))
			{
				// Preparo el array de punts de la curva a dibuixar
				TArray<FSplinePoint> moltsPunts;
				int32 ID = 0;
				int32 TotalPuntsParabola = ResultatParabola.PathData.Num();
				int32 InicialsPuntsMalla = DynamicMeshArray.Num();
				if (TotalPuntsParabola > InicialsPuntsMalla)
				{
					for (int32 count = 0; count < (TotalPuntsParabola - InicialsPuntsMalla); count ++)
					{
						UStaticMeshComponent *DynamicMesh = NewObject<UStaticMeshComponent>(this);
						DynamicMesh->AttachToComponent(VRArrel, FAttachmentTransformRules::KeepRelativeTransform);
						DynamicMesh->SetStaticMesh(LineaMesh);
						DynamicMesh->SetMaterial(0, LineaMaterial);
						DynamicMesh->RegisterComponent();
						DynamicMeshArray.Add(DynamicMesh);
					}
				}
				for (FPredictProjectilePathPointData punt : ResultatParabola.PathData)
				{
					// Cal passar els valors absoluts del resultat a valors relatius a on es
					//   l'origen del teleport.
					FVector posicio = TeleportPath->GetComponentTransform().InverseTransformPosition(punt.Location);
					FSplinePoint puntCurva(ID, posicio, ESplinePointType::Curve);
					moltsPunts.Add(puntCurva);
					DynamicMeshArray[ID]->SetWorldLocation(punt.Location);
					DynamicMeshArray[ID]->SetVisibility(true);
					ID++;
				}
				for (int32 count = ID; count < DynamicMeshArray.Num(); count++)
				{
					DynamicMeshArray[count]->SetVisibility(false);
				}
				TeleportPath->ClearSplinePoints(false);
				TeleportPath->AddPoints(moltsPunts);

				PuntDesti = Projecciotocat.Location;
				resultat = true;
			}
		}
	}
	return resultat;
}
```

We going to use SplineMesh, and no use Sphere.
We need to give points to SplineMesh.
Replace the UStaticMeshComponent with USplineMeshComponent
add include in Cpp
Use LaserMesh and LaserMaterial in BP
Fix Static of SplineMeshComponent default with SetMobility

Use USplineComponent::GetLocalLocationAndTangentAtSplinePoint 
and USplineMeshComponent::SetStartAndEnd
and do the challenge

Trace the Spline
-Get the tangents and locations
begin point - segments
- Iterate over segments
- Set the mesh endpoints
- Hide any extras

```c
	for (int32 count = 0; count < DynamicMeshArray.Num(); count++)
	{
		FVector Posicio, Tangent;
		FVector PosicioFi, TangentFi;
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(count, Posicio, Tangent);
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(count+1, PosicioFi, TangentFi);
		DynamicMeshArray[count]->SetStartAndEnd(Posicio, Tangent, PosicioFi, TangentFi);
	}
```

WARNING!! DynamicMeshArray.Num() without -1, because GetLocalLocationAndTangentAtSplinePoint
watch to return last element if you demand a far away ID. Then conut+1 works.

---
Refactoritzem !!!

Examen : 66

------------------------------
## Extracting Hand Controller Actors

Refactoring all VRCharacter.
Minimaze the code repetition

Create HandController Class C++
Put this code in `BeginPlay`

```c
LeftController = GetWorld()->SpawnActor<AHandController>(HandControllerClass);
if (LeftController != nullptr)
{
    LeftController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
    LeftController->SetHand(EControllerHand::Left);
    LeftController->SetOwner(this); // FIX FOR 4.22
}

RightController = GetWorld()->SpawnActor<AHandController>(HandControllerClass);
if (RightController != nullptr)
{
    RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
    RightController->SetHand(EControllerHand::Right);
    RightController->SetOwner(this); // FIX FOR 4.22
}
```

Extract the controller
- Pull out UMotionControllerComponents into the Hand Controller
HandController.h HandController.cpp
- Create BP Child to set Mesh (quen o cal perque el mesh ya ve de serie en 4.22)
- Fix any build errore that arise

---
### EXTRA: NO TOQUES LO QUE NO HAY QUE TOCAR, COPON.
NO ME TOQUES NAH de la CAMARAH

Altres notes de mirar els BP
    
GetHMDDevideName
SetTrackingOrigin  FloorLevel

SpawnActor with Transform
AttachActorToComponent  VRRoot
Location and Rotation Rule Snap to Target
Scale Rule Keep World 

---

## Querying Overlapping Actors

Climbing start

Controller overlapping a mesh to climb

New kind of moving mechanic

New Blueprint from Actor BP_HandHolds
Add Components : Box Collision and StaticMesh (Insert Cube in slot)
Add tag Climbable

With overlap event we detect than Controller touch object

FActorBeginOverlapSignature OnActorBeginOverlap

// Callbacks
void ActorBeginOverlap (AActor* OverlappedActor, AActor* OtherActor);

//State variable
bool bCanClimb = false;

In BeginPlay of HandController
OnActorBeginOverlap.AddDynamic(this, &[[OurFunction]]);

AActor::GetOverlappingActors

-Query the overlapping actors
-Change the state when we get an event
-Print a message when we can climb

Change Collision from StaticMesh to OverlapAllDynamics
and in BP_Hand Motion Controller, Collision to OverlapAllDynamics

```c
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
	
# In CPP:

# In BeginPlay:
	OnActorBeginOverlap.AddDynamic(this, &AHandController::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AHandController::ActorEndOverlap);

void AHandController::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	bool bNew = CanClimb();
	if (!bCanClimb && bNew)
	{
		UE_LOG(LogTemp, Warning, TEXT("Toco alguna cosa"));
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
```

---

## Hapic Feedback Effects
vibration

Assets: Haptic Feedback Effect (Buffer, Curve, Sound Wave)

Copy Haptic Feedback for BP_Virtual Project (because Asset isn't in new Unreal versions)

APlayerController::PlayHapticEffect

- Find the correct player controller
- Reference the curse asset (use BP for that and use asset on C++)
- Play the haptic effect.

	UPROPERTY(EditDefaultsOnly)
	UHapticFeedbackEffect_Base* CurveHaptic;

In AHandController::ActorBeginOverlap: 

```c
		AVRCharacter* Papa = Cast<AVRCharacter>(GetAttachParentActor());
		if (Papa != nullptr)
		{
			APlayerController* PlayControl = Cast<APlayerController>(Papa->GetController());
			if (PlayControl != nullptr)
			{
				PlayControl->PlayHapticEffect(CurveHaptic, MotionController->GetTrackingSource());
			}
		}
```
		
---
## Climbing Motion (part 1)

Grip and move ourself

Binding input controllers
Go to projects setting and in input add 2 buttons (each controller one) to grip
GripLeft - GripRight

Set inputs in VRCharacter
use functions for each axis

```c
	void GripLeft();
	void ReleaseLeft();
	void GripRight();
	void ReleaseRight();
	
	PlayerInputComponent->BindAction(TEXT("GripLeft"), IE_Pressed, this, &AVRCharacter::GripLeft);
	PlayerInputComponent->BindAction(TEXT("GripLeft"), IE_Released, this, &AVRCharacter::ReleaseLeft);
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Pressed, this, &AVRCharacter::GripRight);
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Released, this, &AVRCharacter::ReleaseRight);
```
	
and create Grip and Release functions in HandController class

Add to HandController
	bool bIsClimbing; //(init as false)
	FVector ClimingStartLocation;

Make the Character Move
- Update the state
- Only climb if we can grip
- Move the pawn in tick
- The controller should stay still

```c
void AHandController::Grip()
{
	if (!bCanClimb) return;

	if (!bIsClimbing)
	{
		bIsClimbing = true;
		ClimingStartLocation = GetActorLocation();
	}
}

void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsClimbing) {
		FVector Delta = GetActorLocation() - ClimingStartLocation;
		GetAttachParentActor()->AddActorWorldOffset(-Delta);
	}
}
```

---
## Climbing Motion (part 2)

Set your climbing wall with more Actors

Setting movement modes in Character
EmovementMode to MOVE_Flying

Pair controllers

```c
void AHandController::PairController(AHandController* Controller)
{
	OtherController = Controller;
	OtherController->OtherController = this;
}
```

In AVRCharacter::BeginPlay():
LeftController->PairController(RightController);

Steal The Climb State
- Only one controlle should climb
- What should happen on grab?
- What should happen on release?
- Can you climb the wall?

```c
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
```

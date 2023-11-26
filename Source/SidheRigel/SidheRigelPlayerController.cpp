// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"

#include "SidheRigelCharacter.h"
#include "SidheRigelGameInstance.h"
#include "SidheRigel/Character/Cold/ColdCharacter.h"
#include "SidheRigel/Character/FairyWing/FairyWingCharacter.h"
#include "SidheRigel/Character/Kerun/KerunCharacter.h"

#include "SidheRigel/State/MoveState.h"

#include "SidheRigelCamera.h"
#include "Blueprint/WidgetLayoutLibrary.h"

ASidheRigelPlayerController::ASidheRigelPlayerController()
{
	static ConstructorHelpers::FClassFinder<APawn> ColdBPClass(TEXT("/Game/Heros/Cold/BP_ColdCharacter"));
	if (ColdBPClass.Class != nullptr)
	{
		ColdPawn = ColdBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<APawn> FairyWingBPClass(TEXT("/Game/Heros/FairyWIng/BP_FairyWingCharacter"));
	if (FairyWingBPClass.Class != nullptr)
	{
		FairyWingPawn = FairyWingBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<APawn> KerunBPClass(TEXT("/Game/Heros/Kerun/BP_KerunCharacter"));
	if (KerunBPClass.Class != nullptr)
	{
		KerunPawn = KerunBPClass.Class;
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	stateMachine = new StateMachine(this);

	bReplicates = true;
}

void ASidheRigelPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ASidheRigelPlayerController, MyPawnClass);
}

void ASidheRigelPlayerController::BeginPlay()
{
	Super::BeginPlay();

	DeterminePawnClass();

	//Set stateMachine
	delete stateMachine;
	stateMachine = new StateMachine(this);

	auto SRCharacter = Cast<ASidheRigelCharacter>(GetCharacter());
	if (SRCharacter)
	{
		//Set Controller
		SRCharacter->sidheRigelController = this;

		//Set CustomTick
		SRCharacter->SetCustomTick();
	}

	//SetSRCamera();
	//SetSRCharacter();
}

FHitResult ASidheRigelPlayerController::GetHitResult()
{
	FHitResult HitResult;

	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	return HitResult;
}

void ASidheRigelPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	/*
	* if (Camera)
	{
		if (!(Camera->GetIsCameraFixed()))
		{
			FVector2D Location = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			if (Location.X <= 1.f)
			{
				Camera->MoveCameraY(-1.f);
			}

			if (Location.X >= Camera->GetScreenX() - 1.f)
			{
				Camera->MoveCameraY(1.f);
			}

			if (Location.Y <= 1.f)
			{
				Camera->MoveCameraX(1.f);
			}

			if (Location.Y >= Camera->GetScreenY() - 1.f)
			{
				Camera->MoveCameraX(-1.f);
			}
		}
	}
	*/
	
}

void ASidheRigelPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Released, this, &ASidheRigelPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("RIghtClick", IE_Pressed, this, &ASidheRigelPlayerController::ClickedRightMouseButton);
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ASidheRigelPlayerController::ClickedLeftMouseButton);

	InputComponent->BindAction("QPress", IE_Pressed, this, &ASidheRigelPlayerController::PressedQButton);
	InputComponent->BindAction("WPress", IE_Pressed, this, &ASidheRigelPlayerController::PressedWButton);
	InputComponent->BindAction("EPress", IE_Pressed, this, &ASidheRigelPlayerController::PressedEButton);
	InputComponent->BindAction("RPress", IE_Pressed, this, &ASidheRigelPlayerController::PressedRButton);
	//InputComponent->BindAction("YPress", IE_Pressed, this, &ASidheRigelPlayerController::PressedYButton);
}

void ASidheRigelPlayerController::OnSetDestinationReleased()
{
	stateMachine->OnRightRelease();
}

void ASidheRigelPlayerController::ClickedRightMouseButton()
{
	stateMachine->OnRightClick();
}

void ASidheRigelPlayerController::ClickedLeftMouseButton()
{
	stateMachine->OnLeftClick();
}

void ASidheRigelPlayerController::PressedQButton()
{
	stateMachine->OnKeyboard(E_SkillState::Q_Ready);
}

void ASidheRigelPlayerController::PressedWButton()
{
	stateMachine->OnKeyboard(E_SkillState::W_Ready);
}

void ASidheRigelPlayerController::PressedEButton()
{
	stateMachine->OnKeyboard(E_SkillState::E_Ready);
}

void ASidheRigelPlayerController::PressedRButton()
{
	stateMachine->OnKeyboard(E_SkillState::R_Ready);
}
/*
* void ASidheRigelPlayerController::PressedYButton()
{
	Camera->SwitchIsCameraFixed();
}
*/


void ASidheRigelPlayerController::DeterminePawnClass_Implementation()
{
	if (IsLocalController()) //Only Do This Locally (NOT Client-Only, since Server wants this too!)
	{
		auto SRGameInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
		/* Use PawnA if the Text File tells us to */
		if (SRGameInstance)
		{
			if (SRGameInstance->CharacterNum == E_Character::Cold)
			{
				ServerSetPawn(ColdPawn);
				return;
			}
			else if (SRGameInstance->CharacterNum == E_Character::FairyWing)
			{
				ServerSetPawn(FairyWingPawn);
				return;
			}
			else if (SRGameInstance->CharacterNum == E_Character::Kerun)
			{
				ServerSetPawn(KerunPawn);
				return;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("InCorrect Character Selected"));

				ServerSetPawn(ColdPawn);
				return;
			}
		}
	}
}

void ASidheRigelPlayerController::ServerSetPawn_Implementation(TSubclassOf<APawn> InPawnClass)
{
	MyPawnClass = InPawnClass;

	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

void ASidheRigelPlayerController::SetSRCharacter()
{
	APawn* SROldPawn = GetPawn();
	APawn* NewPawn = nullptr;
	UGameInstance* gameInstance = GetGameInstance();
	if (!gameInstance) return;
	USidheRigelGameInstance* SRGameInstance = Cast<USidheRigelGameInstance>(gameInstance);
	if (SRGameInstance)
	{
		UWorld* world = GetWorld();
		if (!world) return;

		auto myCharacterType = SRGameInstance->CharacterNum;
		if (myCharacterType == E_Character::Cold)
		{
			NewPawn = world->SpawnActor<AColdCharacter>(ColdBPClassRef, FVector(1500, 2400, 100), FRotator::ZeroRotator);
		}
		else if (myCharacterType == E_Character::FairyWing)
		{
			NewPawn = world->SpawnActor<AFairyWingCharacter>(FVector(1500, 2800, 100), FRotator::ZeroRotator);
		}
		else if (myCharacterType == E_Character::Kerun)
		{
			NewPawn = world->SpawnActor<AKerunCharacter>(FVector(1500, 3200, 100), FRotator::ZeroRotator);
		}
		else
		{
			NewPawn = world->SpawnActor<AColdCharacter>(FVector(1500, 3600, 100), FRotator::ZeroRotator);
		}

		if (NewPawn)
		{
			Possess(NewPawn);
			auto SRCharacter = Cast<ASidheRigelCharacter>(NewPawn);
			if (SRCharacter)
			{
				//Set stateMachine
				delete stateMachine;
				stateMachine = new StateMachine(this);

				//Set Controller
				SRCharacter->sidheRigelController = this;

				//Set CustomTick
				SRCharacter->SetCustomTick();
			}
		}
	}
	if (SROldPawn)
	{
		SROldPawn->Destroy();
	}
}

/*
void ASidheRigelPlayerController::SetSRCamera()
{
	UE_LOG(LogTemp, Warning, TEXT("IN SET_CAMERA :: IN FUNCTION"))
	UWorld* world = GetWorld();
	if (!world) return;
	UE_LOG(LogTemp, Warning, TEXT("IN SET_CAMERA :: World Is Not Null"))
	// Instantiate our camera actor (actor of type AMyCamera in this case) at position (0, 0, 0) with rotation (0, 0, 0)
	AActor* CameraActor = world->SpawnActor(ASidheRigelCamera::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);
	// We have just spawned AMyCamera and assigned it to the variable CameraActor. Since we know it is of type AMyCamera, we can safelty cast CameraActor variable to the AMyCamera class to be able to use its methods
	ASidheRigelCamera* MyCamera = CastChecked<ASidheRigelCamera>(CameraActor);
	// Pass the instantiated player controller to the camera so that it can read the rotation from one
	MyCamera->SetPlayerController(this);
	// Pass instantiated camera to the player controller so that it can be bound to the player the controller possesses
	this->SetCamera(MyCamera);
}
*/

/*
* void ASidheRigelPlayerController::OnPossess(APawn* aPawn)
{
	// It is important to call the super OnPossess method to make sure the default logic also gets executed
	Super::OnPossess(aPawn);
	if (!Camera)
	{
		AActor* CameraActor = GetWorld()->SpawnActor(ASidheRigelCamera::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);
		// We have just spawned AMyCamera and assigned it to the variable CameraActor. Since we know it is of type AMyCamera, we can safelty cast CameraActor variable to the AMyCamera class to be able to use its methods
		ASidheRigelCamera* MyCamera = CastChecked<ASidheRigelCamera>(CameraActor);
		// Pass the instantiated player controller to the camera so that it can read the rotation from one
		MyCamera->SetPlayerController(this);
		// Pass instantiated camera to the player controller so that it can be bound to the player the controller possesses
		this->SetCamera(MyCamera);
	}
	// Set the target of our camera to be equal to the pawn this controller possesses (thus when the controller will possess the player when the game starts, the camera's target will be set to reference that player)
	Camera->SetTargetToFollow(aPawn);
	// Make sure our camera is the one used to present player with view (make sure that our camera will be used as player camera)
	SetViewTarget(Camera);

	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	Camera->SetScreenSize(ViewportSize.X*2.f, ViewportSize.Y*2.f);
}
*/


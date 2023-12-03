// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"

#include "SidheRigelCharacter.h"
#include "SidheRigelGameInstance.h"
#include "SidheRigel/Character/Cold/ColdCharacter.h"
#include "SidheRigel/Character/FairyWing/FairyWingCharacter.h"
#include "SidheRigel/Character/Kerun/KerunCharacter.h"

#include "SidheRigel/State/MoveState.h"

#include "SidheRigelCamera.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

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

	//bAutoManageActiveCameraTarget = false;
}

void ASidheRigelPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASidheRigelPlayerController, MyPawnClass);
	DOREPLIFETIME(ASidheRigelPlayerController, SRCamera);
	DOREPLIFETIME(ASidheRigelPlayerController, myTeam);
}

void ASidheRigelPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("BeginPlay :: Authority Controller is %s"), *GetName())
	}
	DeterminePawnClass();

	//Set stateMachine
	delete stateMachine;
	stateMachine = new StateMachine(this);

	auto SRCharacter = Cast<ASidheRigelCharacter>(GetCharacter());
	if (SRCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay :: Has SRCHaracter"))
		//Set Controller
		SRCharacter->sidheRigelController = this;
		SRCharacter->team = myTeam;

		//Set CustomTick
		SRCharacter->SetCustomTick();
	}

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputModeData.SetHideCursorDuringCapture(false);

	SetInputMode(InputModeData);

	//SetSRCamera();
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

	if (SRCamera)
	{
		if (!(SRCamera->GetIsCameraFixed()))
		{
			FVector2D Location;
			this->GetMousePosition(Location.X, Location.Y);

			if (Location.X <= 1.f)
			{
				SRCamera->MoveCameraY(-1.f);
			}

			if (Location.X >= SRCamera->GetScreenX() - 1.f)
			{
				SRCamera->MoveCameraY(1.f);
			}

			if (Location.Y <= 1.f)
			{
				SRCamera->MoveCameraX(1.f);
			}

			if (Location.Y >= SRCamera->GetScreenY() - 1.f)
			{
				SRCamera->MoveCameraX(-1.f);
			}
		}

		
		if (IsLocalController())
		{
			if (!SRCamera->GetOwner())
			{
				SRCamera->SetOwner(this);
			}
		}
		
	}
	else
	{
		if (GetCharacter() != nullptr)
		{
			ASidheRigelPlayerController* TargetController = Cast<ASidheRigelPlayerController>(GetCharacter()->GetController());
			if (IsLocalController())
			{
				SpawnSRCamera(GetCharacter(), this);
			}
		}
				
	}
		
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
	InputComponent->BindAction("YPress", IE_Pressed, this, &ASidheRigelPlayerController::PressedYButton);
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

void ASidheRigelPlayerController::PressedYButton()
{
	if (SRCamera)
	{
		SRCamera->SwitchIsCameraFixed();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Camera is Null"))
	}
	
}

void ASidheRigelPlayerController::SetSRCameraInClient_Implementation(APawn* aPawn, ASidheRigelPlayerController* controller)
{
	if (aPawn)
	{
		if (!SRCamera)
		{
			UWorld* world = GetWorld();

			if (!world) return;
			// Instantiate our camera actor (actor of type AMyCamera in this case) at position (0, 0, 0) with rotation (0, 0, 0)
			AActor* CameraActor = world->SpawnActor(ASidheRigelCamera::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);
			// We have just spawned AMyCamera and assigned it to the variable CameraActor. Since we know it is of type AMyCamera, we can safelty cast CameraActor variable to the AMyCamera class to be able to use its methods
			ASidheRigelCamera* MyCamera = CastChecked<ASidheRigelCamera>(CameraActor);

			MyCamera->SetOwner(this);
			ASidheRigelPlayerController* TargetController = Cast<ASidheRigelPlayerController>(aPawn->GetController());

			if (this)
			{
				// Pass the instantiated player controller to the camera so that it can read the rotation from one
				MyCamera->SetPlayerController(this);
				// Pass instantiated camera to the player controller so that it can be bound to the player the controller possesses
				this->SetCamera(MyCamera);

			}

		}

		// Set the target of our camera to be equal to the pawn this controller possesses(thus when the controller will possess the player when the game starts, the camera's target will be set to reference that player)
		SRCamera->SetTargetToFollow(aPawn);
		// Make sure our camera is the one used to present player with view (make sure that our camera will be used as player camera)
		SetViewTarget(SRCamera);

		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

		SRCamera->SetScreenSize(ViewportSize.X, ViewportSize.Y);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn is Null"));
	}
}



void ASidheRigelPlayerController::DeterminePawnClass_Implementation()
{
	if (IsLocalController())
	{
		auto SRGameInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
		
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

	auto SRGameInstance = Cast<USidheRigelGameInstance>(GetGameInstance());
	if (SRGameInstance)
	{
		myTeam = SRGameInstance->myTeam;
	}

	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}


void ASidheRigelPlayerController::SetSRCamera_Implementation(APawn* aPawn, class ASidheRigelPlayerController* controller)
{
	
	SetSRCameraInClient(aPawn, controller);

}



void ASidheRigelPlayerController::SpawnSRCamera_Implementation(APawn* aPawn, class ASidheRigelPlayerController* controller)
{
	if (IsLocalPlayerController())
	{
		if (!SRCamera)
		{
			SetSRCamera(aPawn,this);
		}
	}
}



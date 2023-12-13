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
#include "SidheRigel/Character/AI/AISidheRigelCharacter.h"

#include "SidheRigel/State/IdleState.h"
#include "SidheRigel/State/MoveToAttackState.h"
#include "SidheRigel/State/MoveState.h"
#include "SidheRigel/State/AttackWaitState.h"
#include "SidheRigel/State/AttackState.h"
#include "SidheRigel/State/UseSkillState.h"
#include "SidheRigel/State/StunState.h"
#include "SidheRigel/State/DieState.h"

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

	static ConstructorHelpers::FObjectFinder<UBlueprint> BPAI(TEXT("/Game/Heros/AI/BP_AISidheRigelCharacter"));
	if (BPAI.Object)
	{
		AIClass = (UClass*)BPAI.Object->GeneratedClass;
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	bReplicates = true;

	//bAutoManageActiveCameraTarget = false;
}

void ASidheRigelPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASidheRigelPlayerController, MyPawnClass);
	DOREPLIFETIME(ASidheRigelPlayerController, SRCamera);
	DOREPLIFETIME(ASidheRigelPlayerController, myTeam);
	//DOREPLIFETIME(ASidheRigelPlayerController, stateMachine);
}

void ASidheRigelPlayerController::BeginPlay()
{
	Super::BeginPlay();

	DeterminePawnClass();

	if (IsLocalController())
	{
		InitializeState();

		auto SRCharacter = Cast<ASidheRigelCharacter>(GetCharacter());
		if (SRCharacter)
		{
			//Set Controller
			SRCharacter->sidheRigelController = this;
		}
	}

	GetWorldTimerManager().SetTimer(stateUpdateTimer, this, &ASidheRigelPlayerController::IE_Update, deltaTime, true);

	//SetInputMode
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputModeData.SetHideCursorDuringCapture(false);

	SetInputMode(InputModeData);
}

void ASidheRigelPlayerController::OnPossess(APawn* aPawn)	//InServerFunction
{
	Super::OnPossess(aPawn);

	ASidheRigelCharacter* SRCharacter = Cast<ASidheRigelCharacter>(aPawn);
	if (SRCharacter)
	{
		//Set Controller
		SRCharacter->team = myTeam;

		//Set Player Start Location
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

		AActor* RedStart = nullptr;

		for (auto playerStart : PlayerStarts)
		{
			auto playerStartTag = Cast<APlayerStart>(playerStart)->PlayerStartTag;

			if (playerStartTag.ToString() == "Red")
				RedStart = playerStart;

			if (((playerStartTag.ToString() == "Blue") && (SRCharacter->GetTeam() == E_Team::Blue)) ||
				((playerStartTag.ToString() == "Red") && (SRCharacter->GetTeam() == E_Team::Red)))
			{
				SRCharacter->spawnLocation = playerStart->GetActorLocation();
				SRCharacter->Server_MoveToStartLocation(playerStart->GetActorLocation());

				break;
			}
		}

		if (Cast<USidheRigelGameInstance>(GetGameInstance())->bIsSingleGame)
		{
			//UE_LOG(LogTemp, Warning, TEXT("This Game Is Single GAme"));

			//UWorld* World = GetWorld();
			//if (World)
			//{
			//	FTransform SpawnTransform;
			//	SpawnTransform.SetLocation(RedStart->GetActorLocation());
			//	SpawnTransform.SetRotation(RedStart->GetActorRotation().Quaternion());

			//	AAISidheRigelCharacter* AI = World->SpawnActorDeferred<AAISidheRigelCharacter>(AIClass, SpawnTransform);
			//	if (AI)
			//	{
			//		AI->SetTeam(E_Team::Red);
			//		AI->spawnLocation = RedStart->GetActorLocation();
			//	}
			//	AI->FinishSpawning(SpawnTransform);
			//}
		}
	}
}

void ASidheRigelPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//Camera Input
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

	//State
	if (myCharacter)
	{
		if (attackDelay > 0)
		{
			attackDelay -= DeltaTime;
		}
		if (frontDelay > 0)
		{
			frontDelay -= DeltaTime;
		}
		if (skillDelay > 0)
		{
			skillDelay -= DeltaTime;
		}
	}
	else
	{
		myCharacter = Cast<ASidheRigelCharacter>(GetPawn());
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
	InputComponent->BindAction("SpacePress", IE_Pressed, this, &ASidheRigelPlayerController::PressedSpaceButton);
	InputComponent->BindAction("APress", IE_Pressed, this, &ASidheRigelPlayerController::PressedAButton);
}

void ASidheRigelPlayerController::OnSetDestinationReleased()
{
	currentState->OnRightRelease();
}

void ASidheRigelPlayerController::ClickedRightMouseButton()
{
	//UE_LOG(LogTemp, Error, TEXT("OnRightClick"))
	//SkillCancel
	bAttackReady = false;
	bSkillReady = false;
	currentSkill = E_SkillState::Skill_Null;
	if (myCharacter)
	{
		myCharacter->skillRange->SetVisibility(false);
	}
	else
	{
		myCharacter = Cast<ASidheRigelCharacter>(GetCharacter());
		UE_LOG(LogTemp, Error, TEXT("ERROR CHARACTER NULL IN STATEMACHINE"))
	}

	currentState->OnRightClick();
}

void ASidheRigelPlayerController::ClickedLeftMouseButton()
{
	currentState->OnLeftClick();
}

void ASidheRigelPlayerController::PressedQButton()
{
	OnKeyboard(E_SkillState::Q_Ready);
}

void ASidheRigelPlayerController::PressedWButton()
{
	OnKeyboard(E_SkillState::W_Ready);
}

void ASidheRigelPlayerController::PressedEButton()
{
	OnKeyboard(E_SkillState::E_Ready);
}

void ASidheRigelPlayerController::PressedRButton()
{
	OnKeyboard(E_SkillState::R_Ready);
}

void ASidheRigelPlayerController::PressedYButton()
{
	if (SRCamera)
	{
		SRCamera->SwitchIsCameraFixed();
	}
}

void ASidheRigelPlayerController::PressedSpaceButton()
{
	if (SRCamera)
	{
		SRCamera->SetCameraInActorLocation();
	}
}

void ASidheRigelPlayerController::PressedAButton()
{
	bSkillReady = false;
	currentSkill = E_SkillState::Skill_Null;

	if (!myCharacter)
	{
		myCharacter = Cast<ASidheRigelCharacter>(GetCharacter());
	}
	if (myCharacter->stunTime > 0)
	{
		return;
	}

	bAttackReady = true;
	float currentRange = myCharacter->GetRange();
	myCharacter->skillRange->SetRelativeScale3D(FVector(currentRange / 100, currentRange / 100, 1));

	myCharacter->skillRange->SetVisibility(true);
}

void ASidheRigelPlayerController::IE_Update()
{
	if (currentState)
	{
		currentState->Update(deltaTime);
	}
}

void ASidheRigelPlayerController::InitializeState()
{
	Idle = NewObject<UIdleState>();
	Idle->controller = this;
	MoveToAttack = NewObject<UMoveToAttackState>();
	MoveToAttack->controller = this;
	Move = NewObject<UMoveState>();
	Move->controller = this;
	AttackWait = NewObject<UAttackWaitState>();
	AttackWait->controller = this;
	Attack = NewObject<UAttackState>();
	Attack->controller = this;
	UseSkill = NewObject<UUseSkillState>();
	UseSkill->controller = this;

	Die = NewObject<UDieState>();
	Die->controller = this;

	currentState = Idle;
}

FHitResult ASidheRigelPlayerController::GetHitResult()
{
	FHitResult HitResult;

	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	return HitResult;
}

AActor* ASidheRigelPlayerController::GetCloseActorToMouse()
{
	AActor* res = nullptr;

	FHitResult Hit = GetHitResult();

	if (myCharacter)
	{
		float dist = 50000;	//temp MAX
		for (auto actor : myCharacter->InRangeActors)
		{
			float tempDist = (Hit.Location - actor->GetActorLocation()).Length();
			if (dist > tempDist)
			{
				dist = tempDist;
				res = actor;
			}

		}
	}
	return res;
}

void ASidheRigelPlayerController::ChangeState(UState* NextState)
{
	previousState = currentState;
	currentState->OnEnd();
	if (NextState)
	{
		NextState->OnBegin();
		currentState = NextState;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NextState Is Null"))
	}
}

void ASidheRigelPlayerController::ChangePreviousState()
{
	currentState->OnEnd();
	previousState->OnBegin();

	if (previousState == UseSkill)
	{
		previousState = Idle;
	}

	currentState = previousState;
}

UState* ASidheRigelPlayerController::GetCurrentState()
{
	return currentState;
}

void ASidheRigelPlayerController::OnKeyboard(E_SkillState SkillState)
{
	bAttackReady = false;

	currentSkill = SkillState;
	if (!myCharacter)
	{
		myCharacter = Cast<ASidheRigelCharacter>(GetCharacter());
	}
	if (myCharacter->silenceTime > 0)
	{
		return;
	}

	if (!myCharacter->skills.Contains(currentSkill))
	{
		currentSkill = E_SkillState::Skill_Null;
		return;
	}

	bSkillReady = true;
	float currentSkillRange = myCharacter->skills[currentSkill]->GetRange();
	myCharacter->skillRange->SetRelativeScale3D(FVector(currentSkillRange / 100, currentSkillRange / 100, 1));

	myCharacter->skillRange->SetVisibility(true);

	currentState->OnKeyboard(SkillState);
}

void ASidheRigelPlayerController::HasAttackEnemy()
{
	//Move & Attack
	AActor* HitActor = GetHitResult().GetActor();
	if (HitActor)
	{
		IDamagable* DamagableActor = Cast<IDamagable>(HitActor);
		if (DamagableActor)
		{
			ITeam* TeamActor = Cast<ITeam>(HitActor);
			if (!myCharacter)
			{
				myCharacter = Cast<ASidheRigelCharacter>(GetCharacter());
			}
			if (TeamActor && TeamActor->GetTeam() != myCharacter->GetTeam())
			{
				target = HitActor;
				ChangeState(MoveToAttack);
			}
		}
		else
		{
			location = GetHitResult().Location;
			ChangeState(Move);
		}
	}
}

void ASidheRigelPlayerController::ChangeCurrentSkill(E_SkillState SkillState)
{
	currentSkill = SkillState;

	//스킬이 없으면 (케룬 E 등) return
	if (!myCharacter)
	{
		myCharacter = Cast<ASidheRigelCharacter>(GetCharacter());
	}
	if (!myCharacter->skills.Contains(currentSkill))
	{
		currentSkill = E_SkillState::Skill_Null;
		return;
	}

	//예외처리
	auto skill = myCharacter->skills[SkillState];
	if (skill == nullptr)
	{
		currentSkill = E_SkillState::Skill_Null;
		return;
	}

	//Check Cooldown, Mana
	if (skill->GetCooldown() <= 0 && skill->hasEnoughMana())
	{
		currentSkill = SkillState;
		bSkillReady = true;

		//Check Instant cast
		if (skill->IsInstantCast() && skill->CanUse())
		{
			ChangeState(UseSkill);
		}
	}
	else //현재 사용 불가능 (마나 부족 || 쿨타임)
	{
		currentSkill = E_SkillState::Skill_Null;
		bSkillReady = false;
		myCharacter->skillRange->SetVisibility(false);
	}
}

void ASidheRigelPlayerController::DeterminePawnClass_Implementation()
{
	if (IsLocalController())
	{
		auto SRGameInstance = Cast<USidheRigelGameInstance>(GetGameInstance());

		if (SRGameInstance)
		{
			ServerSetTeam(SRGameInstance->myTeam);

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
				UE_LOG(LogTemp, Error, TEXT("InCorrect Character Selected"));

				ServerSetPawn(ColdPawn);
				return;
			}
		}
	}
}

void ASidheRigelPlayerController::ServerSetTeam_Implementation(E_Team team)
{
	myTeam = team;
}

void ASidheRigelPlayerController::ServerSetPawn_Implementation(TSubclassOf<APawn> InPawnClass)
{
	MyPawnClass = InPawnClass;

	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

void ASidheRigelPlayerController::SpawnSRCamera_Implementation(APawn* aPawn, class ASidheRigelPlayerController* controller)
{
	if (IsLocalPlayerController())
	{
		if (!SRCamera)
		{
			SetSRCamera(aPawn, this);
		}
	}
}

void ASidheRigelPlayerController::SetSRCamera_Implementation(APawn* aPawn, class ASidheRigelPlayerController* controller)
{

	SetSRCameraInClient(aPawn, controller);

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
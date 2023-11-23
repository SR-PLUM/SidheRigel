// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "SidheRigelCharacter.h"

#include "SidheRigel/State/MoveState.h"

#include "Engine/World.h"

#include "SidheRigelCamera.h"

ASidheRigelPlayerController::ASidheRigelPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	stateMachine = new StateMachine(this);
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
	Camera->SwitchIsCameraFixed();
}

void ASidheRigelPlayerController::OnPossess(APawn* aPawn)
{
	// It is important to call the super OnPossess method to make sure the default logic also gets executed
	Super::OnPossess(aPawn);
	// Set the target of our camera to be equal to the pawn this controller possesses (thus when the controller will possess the player when the game starts, the camera's target will be set to reference that player)
	Camera->SetTargetToFollow(aPawn);
	// Make sure our camera is the one used to present player with view (make sure that our camera will be used as player camera)
	SetViewTarget(Camera);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess"));
}

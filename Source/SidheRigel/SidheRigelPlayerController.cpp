// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "SidheRigelCharacter.h"

#include "SidheRigel/State/MoveState.h"

#include "Engine/World.h"

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

	stateMachine->Update(DeltaTime);
}

void ASidheRigelPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Released, this, &ASidheRigelPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("RIghtClick", IE_Pressed, this, &ASidheRigelPlayerController::ClickedRightMouseButton);
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ASidheRigelPlayerController::ClickedLeftMouseButton);
}

void ASidheRigelPlayerController::OnSetDestinationReleased()
{
	stateMachine->OnRightRelease();

	//// If it was a short press
	//if(FollowTime <= ShortPressThreshold)
	//{
	//	// We look for the location in the world where the player has pressed the input
	//	FVector HitLocation = FVector::ZeroVector;
	//	FHitResult Hit;
	//	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	//	HitLocation = Hit.Location;
	//	
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	//}
}

void ASidheRigelPlayerController::ClickedRightMouseButton()
{
	stateMachine->OnRightClick();
}

void ASidheRigelPlayerController::ClickedLeftMouseButton()
{
	stateMachine->OnLeftClick();
	/*FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, HitResult);

	auto MyPawn = Cast<ASidheRigelCharacter>(GetPawn());
	if (MyPawn)
	{
		MyPawn->UseSkill(HitResult);
	}*/
}

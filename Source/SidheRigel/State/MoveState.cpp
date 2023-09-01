// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"

MoveState::MoveState(StateMachine* StateMachine) : State(StateMachine)
{
}

MoveState::~MoveState()
{
}

void MoveState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("MOVE BEGIN"));

	if (stateMachine && stateMachine->playerController)
	{
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());

		// We flag that the input is being pressed
		bInputPressed = true;
		FollowTime = 0.f;
		// Just in case the character was moving because of a previous short press we stop it
		stateMachine->playerController->StopMovement();
	}
}

void MoveState::Update(float DeltaTime)
{
	if (stateMachine && stateMachine->playerController && myCharacter)
	{
		if (((myCharacter->GetActorLocation() == stateMachine->location) || myCharacter->GetVelocity().Size() < 0.4f) && bInputPressed == false)
		{
			stateMachine->ChangeState(stateMachine->Idle);
		}
		else
		{
			if (bInputPressed)	//LongClick
			{
				FollowTime += DeltaTime;

				// Refresh the Location
				FHitResult Hit;
				stateMachine->playerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
				stateMachine->location = Hit.Location;

				FVector WorldDirection = (stateMachine->location - myCharacter->GetActorLocation()).GetSafeNormal();
				myCharacter->AddMovementInput(WorldDirection, 1.f, false);
			}
			else
			{
				FollowTime = 0.f;
			}
		}
	}
}

void MoveState::OnRightClick()
{
	bInputPressed = false;

	stateMachine->Idle->OnRightClick();
}

void MoveState::OnRightRelease()
{
	UE_LOG(LogTemp, Warning, TEXT("MOVE RIGHTRELEASE"));
	// Player is no longer pressing the input
	bInputPressed = false;

	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->location);
	}
}

void MoveState::OnLeftClick()
{
}

void MoveState::OnKeyboard()
{
}

void MoveState::OnEnd()
{
}

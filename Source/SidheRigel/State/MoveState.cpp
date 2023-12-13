// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveState.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"

UMoveState::UMoveState()
{
}

UMoveState::~UMoveState()
{
}

void UMoveState::OnBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("UMoveState :: Begin"));
	if (controller)
	{
		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());

		//input is being pressed
		bInputPressed = true;
		//previous move will stop
		controller->StopMovement();

		controller->target = nullptr;

		if (controller->bAttackReady)
		{
			controller->bAttackReady = false;
			bInputPressed = false;
		}
	}
}

void UMoveState::Update(float DeltaTime)
{
	if (controller && myCharacter)
	{
		//(도착 && 마우스 우클릭을 풀었는가)
		if (((myCharacter->GetActorLocation() - controller->location).Length() <= 50) && bInputPressed == false)
		{
			controller->ChangeState(controller->Idle);
		}
		else
		{
			if (bInputPressed)	//LongClick
			{
				// Refresh the Location
				controller->location = controller->GetHitResult().Location;

				FVector WorldDirection = (controller->location - myCharacter->GetActorLocation()).GetSafeNormal();
				
				//UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->location);
			}

			myCharacter->Server_MoveToPoint(controller->location);
		}
	}
}

void UMoveState::OnRightClick()
{
	controller->HasAttackEnemy();
}

void UMoveState::OnRightRelease()
{
	bInputPressed = false;

	myCharacter->Server_MoveToPoint(controller->location);
	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->location);
}

void UMoveState::OnLeftClick()
{
	if (controller->bSkillReady)
	{
		if (controller->currentSkill != E_SkillState::Skill_Null)
		{
			if(myCharacter->skills[controller->currentSkill]->CanUse())
			{
				controller->ChangeState(controller->UseSkill);
			}
		}
	}
	else if (controller->bAttackReady)
	{
		auto tempTarget = controller->GetCloseActorToMouse();

		myCharacter->skillRange->SetVisibility(false);

		if (tempTarget != nullptr)
		{
			controller->bAttackReady = false;

			controller->target = tempTarget;
			controller->ChangeState(controller->MoveToAttack);
		}
		else if (tempTarget == nullptr)
		{
			controller->location = controller->GetHitResult().Location;
			controller->ChangeState(controller->Move);
		}
	}
}

void UMoveState::OnKeyboard(E_SkillState SkillState)
{
	controller->ChangeCurrentSkill(SkillState);
}

void UMoveState::OnEnd()
{
}

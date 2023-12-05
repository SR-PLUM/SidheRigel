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
	}
}

void UMoveState::Update(float DeltaTime)
{
	if (controller && myCharacter)
	{
		//(도착 && 마우스 우클릭을 풀었는가)
		if (((myCharacter->GetActorLocation() == controller->location) || myCharacter->GetVelocity().Size() < 0.4f) && bInputPressed == false)
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
				
				myCharacter->Server_MoveToPoint(controller->location);
				//UAIBlueprintHelperLibrary::SimpleMoveToLocation(stateMachine->playerController, stateMachine->location);
			}
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
}

void UMoveState::OnKeyboard(E_SkillState SkillState)
{
	controller->ChangeCurrentSkill(SkillState);
}

void UMoveState::OnEnd()
{
}

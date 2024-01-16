// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"
#include "SidheRigel/Interface/Damagable.h"

UIdleState::UIdleState()
{
}

UIdleState::~UIdleState()
{
}

void UIdleState::OnBegin()
{
	if (controller)
	{
		controller->StopMovement();

		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());

		controller->target = nullptr;
		controller->bSkillReady = false;
		controller->bAttackReady = false;
		controller->location = FVector::Zero();
		myCharacter->skillRange->SetVisibility(false);
	}
}

void UIdleState::Update(float DeltaTime)
{
}

void UIdleState::OnRightClick()
{
	controller->HasAttackEnemy();
}

void UIdleState::OnRightRelease()
{
}

void UIdleState::OnLeftClick()
{
	if (!myCharacter)
		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());

	if (controller->bSkillReady)
	{
		if (controller->currentSkill != E_SkillState::Skill_Null)
		{
			if (myCharacter->skills[controller->currentSkill]->CanUse())
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

void UIdleState::OnKeyboard(E_SkillState SkillState)
{
	controller->ChangeCurrentSkill(SkillState);
}

void UIdleState::OnEnd()
{
	if (myCharacter)
	{
		if (myCharacter->isRecall == true)
		{
			myCharacter->RemoveRecallParticle();

			UAnimInstance* AnimInstance = myCharacter->GetMesh()->GetAnimInstance();
			if (AnimInstance && myCharacter->RecallMontage)
			{
				myCharacter->GetWorldTimerManager().ClearTimer(myCharacter->RecallTimer);
				AnimInstance->Montage_Stop(0.f);
			}
		}
	}
}

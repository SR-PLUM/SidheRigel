// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackState.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"

UAttackState::UAttackState()
{
}

UAttackState::~UAttackState()
{
}

void UAttackState::OnBegin()
{
	UE_LOG(LogTemp,Warning,TEXT("AttackState :: Begin"))
	if (controller)
	{
		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());

		if (controller->target != nullptr)
		{
			if (Cast<IDamagable>(controller->target)->GetHP() <= 0)
			{
				controller->target = nullptr;
				controller->ChangeState(controller->Idle);
				return;
			}
		}

		///Set Attack FrontDelay///
		if (myCharacter)
		{
			controller->frontDelay = myCharacter->frontDelay;
		}

		//Turn to Target
		if (controller->target)
		{
			FVector ForwardDirection = ((controller->target->GetActorLocation() - myCharacter->GetActorLocation()) * FVector(1, 1, 0)).GetSafeNormal();
			myCharacter->SetActorRotation(ForwardDirection.Rotation());
		}
	}
}

void UAttackState::Update(float DeltaTime)
{
	if (controller->frontDelay <= 0 && myCharacter)
	{
		//Attack
		myCharacter->Attack(controller->target);

		controller->attackDelay = 1 / myCharacter->GetAttackSpeed();

		controller->ChangeState(controller->MoveToAttack);
	}
	if (controller->target != nullptr)
	{
		if (Cast<IDamagable>(controller->target)->GetHP() <= 0)
		{
			controller->target = nullptr;
			controller->ChangeState(controller->Idle);
			return;
		}
	}
}

void UAttackState::OnRightClick()
{
	controller->HasAttackEnemy();
}

void UAttackState::OnRightRelease()
{
}

void UAttackState::OnLeftClick()
{
	//스킬 준비 상태
	if (controller->bSkillReady)
	{
		//지정된 스킬이 있는 상태
		if (controller->currentSkill != E_SkillState::Skill_Null)
		{
			//스킬이 사용가능 한 상태 (사거리, 타겟팅 대상 등)
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

		if (tempTarget != controller->target)
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

void UAttackState::OnKeyboard(E_SkillState SkillState)
{
	controller->ChangeCurrentSkill(SkillState);
}

void UAttackState::OnEnd()
{
	
}
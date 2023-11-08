// Fill out your copyright notice in the Description page of Project Settings.


#include "UseSkillState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Skill.h"

UseSkillState::UseSkillState(StateMachine* StateMachine) : State(StateMachine)
{
}

UseSkillState::~UseSkillState()
{
}

void UseSkillState::OnBegin()
{
	//Stop To Use Skill
	if (stateMachine->playerController)
	{
		stateMachine->playerController->StopMovement();

		//USE SKILL
		myCharacter = Cast<ASidheRigelCharacter>(stateMachine->playerController->GetPawn());
		if (myCharacter)
		{
			//Get SkillDelay
			stateMachine->skillDelay = myCharacter->skills[stateMachine->currentSkill]->GetSkillDelay();

			//Cast Skill
			FHitResult Hit;
			stateMachine->playerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);

			if (myCharacter->skills.Contains(stateMachine->currentSkill))
			{
				if (!(myCharacter->skills[stateMachine->currentSkill]->IsTargeting()))
				{
					FVector targetVector = Hit.Location - myCharacter->GetActorLocation();
					if (myCharacter->skills[stateMachine->currentSkill]->GetRange() < targetVector.Size())
					{
						targetVector.Normalize();

						targetVector *= myCharacter->skills[stateMachine->currentSkill]->GetRange();

						Hit.Location = myCharacter->GetActorLocation() + targetVector;
					}
				}
				myCharacter->skills[stateMachine->currentSkill]->OnUse(Hit);
			}

			//Set Character Rotation
			FVector ForwardDirection = ((Hit.Location - myCharacter->GetActorLocation()) * FVector(1, 1, 0)).GetSafeNormal();
			myCharacter->SetActorRotation(ForwardDirection.Rotation());

			//Skill Cooldown
			myCharacter->skills[stateMachine->currentSkill]->SetCooldown();

			//Return Flag
			stateMachine->bSkillReady = false;
			stateMachine->currentSkill = E_SkillState::Skill_Null;

			myCharacter->skillRange->SetVisibility(false);
		}
	}
}

void UseSkillState::Update(float DeltaTime)
{
	if (stateMachine->skillDelay > 0)
	{
		stateMachine->skillDelay -= DeltaTime;
	}
	else
	{
		stateMachine->ChangePreviousState();
	}
}

void UseSkillState::OnRightClick()
{
}

void UseSkillState::OnRightRelease()
{
}

void UseSkillState::OnLeftClick()
{
}

void UseSkillState::OnKeyboard(E_SkillState SkillState)
{
}

void UseSkillState::OnEnd()
{
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UseSkillState.h"

#include "StateMachine.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Skill.h"

UUseSkillState::UUseSkillState()
{
}

UUseSkillState::~UUseSkillState()
{
}

void UUseSkillState::OnBegin()
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

			FVector ForwardDirection = ((Hit.Location - myCharacter->GetActorLocation()) * FVector(1, 1, 0)).GetSafeNormal();

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
			myCharacter->SetActorRotation(ForwardDirection.Rotation());

			//Skill Cooldown
			myCharacter->skills[stateMachine->currentSkill]->SetCooldown();

			myCharacter->skillRange->SetVisibility(false);
		}
	}
}

void UUseSkillState::Update(float DeltaTime)
{
	if (stateMachine->skillDelay > 0)
	{
		stateMachine->skillDelay -= DeltaTime;
	}
	else
	{
		//Return Flag
		stateMachine->bSkillReady = false;
		stateMachine->currentSkill = E_SkillState::Skill_Null;

		stateMachine->ChangePreviousState();
	}
}

void UUseSkillState::OnRightClick()
{
}

void UUseSkillState::OnRightRelease()
{
}

void UUseSkillState::OnLeftClick()
{
}

void UUseSkillState::OnKeyboard(E_SkillState SkillState)
{
}

void UUseSkillState::OnEnd()
{
}

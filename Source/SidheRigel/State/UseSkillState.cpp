// Fill out your copyright notice in the Description page of Project Settings.


#include "UseSkillState.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Skill.h"
#include "SidheRigel/SidheRigelPlayerController.h"

UUseSkillState::UUseSkillState()
{
}

UUseSkillState::~UUseSkillState()
{
}

void UUseSkillState::OnBegin()
{
	//Stop To Use Skill
	if (controller)
	{
		controller->StopMovement();

		//USE SKILL
		myCharacter = Cast<ASidheRigelCharacter>(controller->GetPawn());
		if (myCharacter)
		{
			//Get SkillDelay
			controller->skillDelay = myCharacter->skills[controller->currentSkill]->GetSkillDelay();

			//Cast Skill
			FHitResult Hit = controller->GetHitResult();

			FVector ForwardDirection = ((Hit.Location - myCharacter->GetActorLocation()) * FVector(1, 1, 0)).GetSafeNormal();

			if (myCharacter->skills.Contains(controller->currentSkill))
			{
				if (!(myCharacter->skills[controller->currentSkill]->IsTargeting()))
				{
					//크기 최대거리로 맞추기
					FVector targetVector = Hit.Location - myCharacter->GetActorLocation();
					if (myCharacter->skills[controller->currentSkill]->GetRange() < targetVector.Size())
					{
						targetVector.Normalize();

						targetVector *= myCharacter->skills[controller->currentSkill]->GetRange();

						Hit.Location = myCharacter->GetActorLocation() + targetVector;
					}
				}
				myCharacter->skills[controller->currentSkill]->OnUse(Hit);
			}
			//Set Character Rotation
			myCharacter->SetActorRotation(ForwardDirection.Rotation());

			//Skill Cooldown
			myCharacter->skills[controller->currentSkill]->SetCooldown();

			myCharacter->skillRange->SetVisibility(false);
		}
	}
}

void UUseSkillState::Update(float DeltaTime)
{
	//스킬 애니메이션 시간
	if (controller->skillDelay > 0)
	{
		controller->skillDelay -= DeltaTime;
	}
	else
	{
		//Return Flag
		controller->bSkillReady = false;
		controller->currentSkill = E_SkillState::Skill_Null;

		controller->ChangePreviousState();
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

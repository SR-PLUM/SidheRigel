// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdAnimInstance.h"
#include "ColdCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "SidheRigel/SidheRigelPlayerController.h"

void UColdAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ColdCharacter = Cast<AColdCharacter>(TryGetPawnOwner());
	if (ColdCharacter)
	{
		ColdCharacterMovement = ColdCharacter->GetCharacterMovement();

		if (auto SRController = Cast<ASidheRigelPlayerController>(ColdCharacter->GetController()))
		{
			ColdStateMachine = SRController->stateMachine;
		}
	}
}

void UColdAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ColdCharacter)
	{
		//이동
		if (ColdCharacterMovement)
		{
			Speed = UKismetMathLibrary::VSizeXY(ColdCharacterMovement->Velocity);
		}

		
		if (ColdStateMachine)
		{
			//공격
			if (ColdStateMachine->GetCurrentState() == ColdStateMachine->Attack)
			{
				isAttack = true;
			}
			else
			{
				isAttack = false;
			}

			//스킬
			if (ColdStateMachine->GetCurrentState() == ColdStateMachine->UseSkill)
			{
				//In AnimationTime
				if (ColdStateMachine->skillDelay > 0)
				{
					//Q Skill
					if (!isQUse && ColdStateMachine->currentSkill == E_SkillState::Q_Ready)
					{
						isQUse = true;
						qAnimationDuration = ColdCharacter->skills[E_SkillState::Q_Ready]->GetSkillDelay();

						ColdCharacter->GetWorldTimerManager().SetTimer(qAnimationTimer,
							FTimerDelegate::CreateLambda([&]()
								{
									isQUse = false;
								}
						), qAnimationDuration, false);
					}

					//W Skill
					if (!isWUse && ColdStateMachine->currentSkill == E_SkillState::W_Ready)
					{
						isWUse = true;
						wAnimationDuration = ColdCharacter->skills[E_SkillState::W_Ready]->GetSkillDelay();

						ColdCharacter->GetWorldTimerManager().SetTimer(wAnimationTimer,
							FTimerDelegate::CreateLambda([&]()
								{
									isWUse = false;
								}
						), wAnimationDuration, false);
					}

					//E Skill
					if (!isEUse && ColdStateMachine->currentSkill == E_SkillState::E_Ready)
					{
						isEUse = true;
						eAnimationDuration = ColdCharacter->skills[E_SkillState::E_Ready]->GetSkillDelay();

						ColdCharacter->GetWorldTimerManager().SetTimer(eAnimationTimer,
							FTimerDelegate::CreateLambda([&]()
								{
									isEUse = false;
								}
						), eAnimationDuration, false);
					}

					//R Skill
					if (!isRUse && ColdStateMachine->currentSkill == E_SkillState::R_Ready)
					{
						isRUse = true;
						rAnimationDuration = ColdCharacter->skills[E_SkillState::R_Ready]->GetSkillDelay();

						ColdCharacter->GetWorldTimerManager().SetTimer(rAnimationTimer,
							FTimerDelegate::CreateLambda([&]()
								{
									isRUse = false;
								}
						), rAnimationDuration, false);
					}
				}
			}
		}
		else
		{
			if (auto SRController = Cast<ASidheRigelPlayerController>(ColdCharacter->GetController()))
			{
				ColdStateMachine = SRController->stateMachine;
			}
		}
	}
}
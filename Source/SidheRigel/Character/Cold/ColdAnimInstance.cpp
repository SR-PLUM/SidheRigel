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

		SRController = Cast<ASidheRigelPlayerController>(ColdCharacter->GetController());
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

		
		if (SRController)
		{
			//공격
			if (SRController->GetCurrentState() == SRController->Attack)
			{
				isAttack = true;
			}
			else
			{
				isAttack = false;
			}

			//스킬
			if (SRController->GetCurrentState() == SRController->UseSkill)
			{
				//In AnimationTime
				if (SRController->skillDelay > 0)
				{
					//Q Skill
					if (!isQUse && SRController->currentSkill == E_SkillState::Q_Ready)
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
					if (!isWUse && SRController->currentSkill == E_SkillState::W_Ready)
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
					if (!isEUse && SRController->currentSkill == E_SkillState::E_Ready)
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
					if (!isRUse && SRController->currentSkill == E_SkillState::R_Ready)
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
			SRController = Cast<ASidheRigelPlayerController>(ColdCharacter->GetController());
		}
	}
}
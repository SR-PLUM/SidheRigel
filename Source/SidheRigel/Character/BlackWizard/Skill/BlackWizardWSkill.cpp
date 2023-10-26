// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackWizardWSkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Interface/Movable.h"

UBlackWizardWSkill::UBlackWizardWSkill()
{
}

UBlackWizardWSkill::~UBlackWizardWSkill()
{
}

void UBlackWizardWSkill::SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 2.f;
	range = 500.f;

	bIsInstantCast = false;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;
}

void UBlackWizardWSkill::OnUse(FHitResult Hit)
{
	if (IMovable* MovableTarget = Cast<IMovable>(character))
	{
		if (character)
		{
			FVector moveDirection = (Hit.Location - character->GetActorLocation()) * FVector(1, 1, 0);

			MovableTarget->MoveVector(moveDirection, force);
		}
	}
}

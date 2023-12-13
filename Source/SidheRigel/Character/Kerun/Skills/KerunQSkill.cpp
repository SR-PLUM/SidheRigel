// Fill out your copyright notice in the Description page of Project Settings.

#include "KerunQSkill.h"
#include "../KerunCharacter.h"
#include "TimerManager.h"
#include "KerunQSkillTalentQuest.h"

UKerunQSkill::UKerunQSkill()
{
	QSkillTalentQuest = NewObject<UKerunQSkillTalentQuest>();
	QSkillTalentQuest->Initialize(this);
}

void UKerunQSkill::QuitQSkill()
{
	character->RemoveAttackSpeed("QSkill");
	IsWorking = false;
	AttackCount = 0;
}

bool UKerunQSkill::CheckAttackCount()
{
	return AttackCount >= MaxAttackCount;
}

void UKerunQSkill::ApplyTalentWhenFullComboHits(AActor* target)
{
	if (CheckAttackCount())
	{
		if (character->IsSelectedTalent[0][1])
		{
			ASidheRigelCharacter* Character = Cast<ASidheRigelCharacter>(target);

			if (IsValid(Character))
			{
				Character->AddDecreseDefencePercent("KerunQSkillDecreaseDefenseTalent", Kerun01DecreaseDefencePercent, Kerun01DecreaseDefenceTime);
			}
			
		}

		if (character->IsSelectedTalent[0][0])
		{
			if (!(QSkillTalentQuest->GetQuestState()))
			{
				QSkillTalentQuest->IncreaseQuestGoal(1);
			}
		}

		QuitQSkill();

	}
}

float UKerunQSkill::GetQDamage()
{
	float amount = AttackCount * Kerun02UpgradeAmount;
	return amount;
}

void UKerunQSkill::SetMaxAttackCount(int32 Value)
{
	MaxAttackCount = Value;
}

void UKerunQSkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillCooldown = 0;
	skillMaxCooldown = 4.f;
	requireMana = 50.f;

	bIsInstantCast = true;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &UKerunQSkill::OnTick, 0.1f, true);
}

void UKerunQSkill::OnUse(FHitResult Hit)
{
	if (AKerunCharacter* KerunCharacter = Cast<AKerunCharacter>(character))
	{
		KerunCharacter->PlayQSkillSound();
		KerunCharacter->PlayQSkillBuffSound();
	}

	character->AddAttackSpeed("QSkill", MaxAttackSpeed);
	IsWorking = true;
	BuffDuration = MaxDuration;
}

void UKerunQSkill::OnTick()
{
	Super::OnTick();

	if (BuffDuration > 0)
	{
		BuffDuration -= 0.1f;

		if (BuffDuration <= 0)
		{
			QuitQSkill();
		}
	}
}

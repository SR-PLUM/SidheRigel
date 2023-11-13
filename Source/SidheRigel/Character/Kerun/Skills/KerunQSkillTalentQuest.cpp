// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunQSkillTalentQuest.h"
#include "KerunQSkill.h"

inline void UKerunQSkillTalentQuest::Initialize(UKerunQSkill* QSkill)
{
	InitTalentQuestProperty(QSkill);
}

void UKerunQSkillTalentQuest::InitTalentQuestProperty(class UKerunQSkill* QSkill)
{
	QSkillRef = QSkill;

	GoalPoint = 10;
}

int32 UKerunQSkillTalentQuest::GetQAttackCount()
{
	return UpgradeQAttackCount;
}

void UKerunQSkillTalentQuest::OnCompleted()
{
	QSkillRef->SetMaxAttackCount(UpgradeQAttackCount);
}

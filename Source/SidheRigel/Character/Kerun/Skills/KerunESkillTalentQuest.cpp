// Fill out your copyright notice in the Description page of Project Settings.


#include "KerunESkillTalentQuest.h"
#include "../KerunCharacter.h"

void UKerunESkillTalentQuest::InitTalentQuestProperty(AKerunCharacter* character)
{
	Character = character;
}

void UKerunESkillTalentQuest::OnCompleted()
{
	IsCompleted = true;

	Character->SetEMaxStack(UpgradeEMaxCount);
	Character->SetEAddAmount(UpgradeEAddAmount);
}

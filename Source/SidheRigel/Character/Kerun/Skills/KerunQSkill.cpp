// Fill out your copyright notice in the Description page of Project Settings.

#include "KerunQSkill.h"

UKerunQSkill::UKerunQSkill()
{
}

UKerunQSkill::~UKerunQSkill()
{
}

void UKerunQSkill::ImproveAttackSpeed(TMap<FString, float>& AttackSpeed)
{
	AttackSpeed.Add("QSkill", MaxAttackSpeed);
}

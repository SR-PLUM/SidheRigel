// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeMinion.h"

ARangeMinion::ARangeMinion()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WRMesh(TEXT("/Game/Minion/Minion_Lane_Ranged_Dawn"));
	if (WRMesh.Succeeded())
	{
		whiteRangeMinionMesh = WRMesh.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BRMesh(TEXT("/Game/Minion/Minion_Lane_Ranged_Dusk"));
	if (BRMesh.Succeeded())
	{
		blackRangeMinionMesh = BRMesh.Object;
	}
}

void ARangeMinion::BeginPlay()
{
	Super::BeginPlay();

	if (GetTeam() == E_Team::Blue)
	{
		GetMesh()->SetSkeletalMesh(whiteRangeMinionMesh);
	}
	else
	{
		GetMesh()->SetSkeletalMesh(blackRangeMinionMesh);
	}
}

void ARangeMinion::InitProperty()
{
	maxHp = 296;
	hp = maxHp;
	projectileDamage = 24;
	attackDelay = 0.667;
	range = 550;
	gold = 20;
	exp = 30;
}
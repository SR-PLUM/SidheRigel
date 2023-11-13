// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeMinion.h"

void ARangeMinion::InitProperty()
{
	maxHp = 100;
	hp = maxHp;
	projectileDamage = 5;
	attackDelay = 1.5;
	range = 500;
	gold = 20;
	exp = 60;
}
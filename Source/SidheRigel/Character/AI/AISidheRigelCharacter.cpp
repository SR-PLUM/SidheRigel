// Fill out your copyright notice in the Description page of Project Settings.


#include "AISidheRigelCharacter.h"

#include "Components/SphereComponent.h"

void AAISidheRigelCharacter::BeginPlay()
{
	ACharacter::BeginPlay();

	InitProperty();

	InitStatSummary();

	detectRange->OnComponentBeginOverlap.AddDynamic(this, &ASidheRigelCharacter::OnEnterEnemy);
	detectRange->OnComponentEndOverlap.AddDynamic(this, &ASidheRigelCharacter::OnExitEnemy);

	GetWorldTimerManager().SetTimer(GenerateHPTimer, this, &ASidheRigelCharacter::IE_GenerateHP, 1.f, true);
}

void AAISidheRigelCharacter::SetTeam(E_Team setTeam)
{
	team = setTeam;
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/KerunQSkill.h"
#include "KerunCharacter.h"

AKerunCharacter::AKerunCharacter()
{
	
}

void AKerunCharacter::BeginPlay()
{
	Super::BeginPlay();

	KerunQSkillRef = NewObject<UKerunQSkill>();
}

void AKerunCharacter::Tick(float DeltaTime)
{
	
}

void AKerunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AKerunCharacter::SkillOne()
{
	KerunQSkillRef->ImproveAttackSpeed(attackSpeed);
}

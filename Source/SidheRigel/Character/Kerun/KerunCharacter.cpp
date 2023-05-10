// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Dummy/DummyProjectile.h"
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
	Super::Tick(DeltaTime);
}

void AKerunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AKerunCharacter::SpawnAttackProjectile()
{
	FVector MuzzleLocation = GetActorLocation();
	FRotator MuzzleRotation = GetActorRotation();

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn the projectile at the muzzle.
		ADummyProjectile* Projectile = World->SpawnActor<ADummyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			// Set the projectile's initial trajectory.
			Projectile->Target = target;
			Projectile->AttackDamage = GetAttackDamage();
			Projectile->criticalRate = (float)GetCriticalRate() / 100.f;
			Projectile->criticalDamage = (float)GetCriticalDamage() / 100.f + 1;
		}
	}
}

void AKerunCharacter::SkillOne()
{
	KerunQSkillRef->ImproveAttackSpeed(attackSpeed);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMCharacter.h"
#include "ACMAttackProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

AACMCharacter::AACMCharacter()
{
	InitAttackProjectile();

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AACMCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AACMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AACMCharacter::InitProperty()
{
	range.Add("Debug", 200.f);
	attackDamage.Add("Debug", 5.f);
	attackSpeed.Add("Debug", 1.f);
	criticalRate.Add("Debug", 50);
	criticalDamage.Add("Debug", 50);
	MaxHP.Add("Debug", 100.f);
	generateHealthPoint.Add("Debug", 0.2f);
	lifeSteal.Add("Debug", 5.f);
	protectPower.Add("Debug", 20);

	currentHP = GetMaxHP();
}

void AACMCharacter::SpawnAttackProjectile()
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
		AACMAttackProjectile* Projectile = World->SpawnActor<AACMAttackProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			// Set the projectile's initial trajectory.
			InitProjectileProperty(Projectile);

		}
	}
}

void AACMCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/ACM/BP_ACMAttackProjectile"));
	if (Projectile.Object)
	{
		ProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

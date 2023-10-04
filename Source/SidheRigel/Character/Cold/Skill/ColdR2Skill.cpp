// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdR2Skill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Cold/Skill/ColdR2Projectile.h"

ColdR2Skill::ColdR2Skill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdR2Projectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}
}

ColdR2Skill::~ColdR2Skill()
{
}

void ColdR2Skill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 110.f;
	bIsInstantCast = false;

	character = Character;
	skillstate = SkillState;
}

void ColdR2Skill::OnUse(FHitResult Hit)
{
	FVector PawnToTarget = (Hit.Location - character->GetActorLocation()).GetSafeNormal();
	FVector MuzzleLocation = character->GetActorLocation() + PawnToTarget * 50;
	FRotator MuzzleRotation = PawnToTarget.Rotation();

	UWorld* World = character->GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(MuzzleLocation);
		SpawnTransform.SetRotation(MuzzleRotation.Quaternion());
		SpawnParams.Owner = character;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile at the muzzle.
		AColdR2Projectile* projectile = World->SpawnActorDeferred<AColdR2Projectile>(projectileClass, SpawnTransform);
		if (projectile)
		{
			projectile->projectileOwner = character;
			projectile->startLocation = MuzzleLocation;
			projectile->forwardVector = PawnToTarget;
			projectile->damage = colliderDamage;
		}

		projectile->FinishSpawning(SpawnTransform);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdWSkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Cold/Skill/ColdWProjectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdWParticle.h"

UColdWSkill::UColdWSkill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdWProjectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> particleRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdWParticle"));
	if (particleRef.Object)
	{
		particleClass = (UClass*)particleRef.Object->GeneratedClass;
	}
}

UColdWSkill::~UColdWSkill()
{
}

void UColdWSkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 10.f;
	range = 100.f;
	requireMana = 40.f;

	bIsInstantCast = false;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UColdWSkill::OnUse(FHitResult Hit)
{
	USkill::OnUse(Hit);

	FVector PawnToTarget = (Hit.Location - character->GetActorLocation()).GetSafeNormal();
	FVector MuzzleLocation = character->GetActorLocation() + PawnToTarget*50;
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
		AColdWProjectile* projectile = World->SpawnActorDeferred<AColdWProjectile>(projectileClass, SpawnTransform);
		if (projectile)
		{
			projectile->projectileOwner = character;
			projectile->duration = colliderDuration;
			projectile->damage = colliderDamage;
			projectile->force = colliderForce;
		}
	
		projectile->FinishSpawning(SpawnTransform);

		// Spawn the projectile at the muzzle.
		AColdWParticle* particle = World->SpawnActorDeferred<AColdWParticle>(particleClass, SpawnTransform);
		if (particle)
		{
			particle->particleDuration = particleDuration;
		}

		particle->FinishSpawning(SpawnTransform);
	}
}

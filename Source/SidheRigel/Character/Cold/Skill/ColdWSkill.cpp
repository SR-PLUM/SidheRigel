// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdWSkill.h"

#include "SidheRigel/SidheRigelPlayerController.h"
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

	static ConstructorHelpers::FObjectFinder<UBlueprint> wallRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdWWall"));
	if (wallRef.Object)
	{
		wallClass = (UClass*)wallRef.Object->GeneratedClass;
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

	bIsInstantCast = false;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UColdWSkill::OnUse(FHitResult Hit)
{
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
			projectile->force = colliderForce;

			if (character->IsSelectedTalent[0][1])
				projectile->damage = talentDamage;
			else
				projectile->damage = colliderDamage;

			if (character->IsSelectedTalent[1][0])
			{
				//벽 생성
				FActorSpawnParameters WallSpawnParams;
				FTransform WallSpawnTransform;
				WallSpawnTransform.SetLocation(Hit.Location);
				WallSpawnTransform.SetRotation(MuzzleRotation.Quaternion());
				SpawnParams.Owner = character;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AColdWWall* wall = World->SpawnActorDeferred<AColdWWall>(wallClass, WallSpawnTransform);
				if (wall)
				{
					projectile->wall = wall;
					projectile->wallSpawnTransform = WallSpawnTransform;
				}
			}
			if (character->IsSelectedTalent[1][1])
			{
				//방어력증가
				character->AddDefencePoint("Cold_2_2", 50, 1.5);
			}
			if (character->IsSelectedTalent[1][2])
			{
				//속도증가
				character->AddSpeed("Cold_2_3", 60, 3);
			}
		}
		projectile->FinishSpawning(SpawnTransform);

		AColdWParticle* particle = World->SpawnActorDeferred<AColdWParticle>(particleClass, SpawnTransform);
		if (particle)
		{
			particle->particleDuration = particleDuration;
		}
		particle->FinishSpawning(SpawnTransform);

		
	}
}

float UColdWSkill::GetSkillDelay()
{
	float appliedDelay = skillDelay;

	if (character->IsSelectedTalent[0][1])
	{
		appliedDelay = talentDelay;
	}

	return appliedDelay;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdR2Skill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Cold/Skill/ColdR2Projectile.h"

UColdR2Skill::UColdR2Skill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdR2Projectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}
}

UColdR2Skill::~UColdR2Skill()
{
}

void UColdR2Skill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 20.f;
	skillCooldown = 0.f;
	skillMaxCooldown = 25.f;
	range = 5000.f;
	requireMana = 0; //100

	bIsInstantCast = false;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UColdR2Skill::OnUse(FHitResult Hit)
{
	FVector PawnToTarget = ((Hit.Location - character->GetActorLocation()) * FVector(1,1,0)).GetSafeNormal();
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
			projectile->damage = colliderDamage;
			projectile->duration = colliderDuration;
		}

		projectile->FinishSpawning(SpawnTransform);
	}
}

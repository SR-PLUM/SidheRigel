// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingESkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/FairyWing/FairyWingCharacter.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingEProjectile.h"

UFairyWingESkill::UFairyWingESkill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingEProjectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}
}

UFairyWingESkill::~UFairyWingESkill()
{
}

void UFairyWingESkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 1.f;
	range = 500.f;

	bIsInstantCast = false;
	bIsTargeting = true;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UFairyWingESkill::OnUse(FHitResult Hit)
{
	if (AActor* _target = Hit.GetActor())
	{
		if (projectileClass)
		{			
			FVector MuzzleLocation = character->GetActorLocation();
			FRotator MuzzleRotation = character->GetActorRotation();

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
				AFairyWingEProjectile* projectile = World->SpawnActorDeferred<AFairyWingEProjectile>(projectileClass, SpawnTransform);
				if (projectile)
				{
					projectile->target = _target;
					projectile->damage = colliderDamage;
					projectile->projectileOwner = character;
				}

				projectile->FinishSpawning(SpawnTransform);
			}
		}
	
	}
}

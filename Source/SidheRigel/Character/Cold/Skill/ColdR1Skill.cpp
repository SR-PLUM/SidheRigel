// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdR1Skill.h"

#include "SidheRigel/Character/Cold/Skill/ColdR1Projectile.h"
#include "SidheRigel/SidheRigelCharacter.h"

UColdR1Skill::UColdR1Skill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdR1Projectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}
}

UColdR1Skill::~UColdR1Skill()
{
}

void UColdR1Skill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 110.f;
	bIsInstantCast = false;

	character = Character;
	skillstate = SkillState;
}

void UColdR1Skill::OnUse(FHitResult Hit)
{
	if (AActor* _target = Hit.GetActor())
	{
		if (_target->Tags.Contains("Hero"))
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
					AColdR1Projectile* projectile = World->SpawnActorDeferred<AColdR1Projectile>(projectileClass, SpawnTransform);
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
}

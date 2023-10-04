// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdQSkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Cold/Skill/ColdQProjectile.h"

ColdQSkill::ColdQSkill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdQProjectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}
}

ColdQSkill::~ColdQSkill()
{
}

void ColdQSkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 9.5f;
	bIsInstantCast = false;

	character = Character;
}

void ColdQSkill::OnUse(FHitResult Hit)
{
	if (character == nullptr) return;

	if (AActor* _target = Hit.GetActor())
	{
		if (Cast<IDamagable>(_target) && projectileClass)
		{
			for (int32 i = 0; i < count; i++)
			{
				FTimerHandle projectileGenerateTimer;
				character->GetWorldTimerManager().SetTimer(projectileGenerateTimer,
					FTimerDelegate::CreateLambda([=]()
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
							AColdQProjectile* projectile = World->SpawnActorDeferred<AColdQProjectile>(projectileClass, SpawnTransform);
							if (projectile)
							{
								projectile->target = _target;
								projectile->projectileOwner = character;
								projectile->damage = colliderDamage;
								projectile->speed = colliderSpeed;
							}

							projectile->FinishSpawning(SpawnTransform);
						}
					}), (float)i * colliderDelay, false);
			}
		}
	}
}

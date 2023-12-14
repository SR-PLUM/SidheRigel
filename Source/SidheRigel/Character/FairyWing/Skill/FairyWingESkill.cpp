// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingESkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/FairyWing/FairyWingCharacter.h"
#include "SidheRigel/SidheRigelPlayerController.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingEProjectile.h"

UFairyWingESkill::UFairyWingESkill()
{
	static ConstructorHelpers::FObjectFinder<UClass> projectileRef(TEXT("Blueprint'/Game/Heros/FairyWIng/Skill/BP_FairyWingEProjectile.BP_FairyWingEProjectile_C'"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object;
	}
}

UFairyWingESkill::~UFairyWingESkill()
{
}

void UFairyWingESkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 0.2f;
	skillCooldown = 0;
	skillMaxCooldown = 5.f;
	range = 500.f;
	requireMana = 40.f;

	bIsInstantCast = false;
	bIsTargeting = true;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UFairyWingESkill::OnUse(FHitResult Hit)
{
	USkill::OnUse(Hit);

	if (AActor* _target = Hit.GetActor())
	{
		if (AFairyWingCharacter* FairyWingCharacter = Cast<AFairyWingCharacter>(character))
		{
			FairyWingCharacter->PlayESkillMontage();
			FairyWingCharacter->PlayESkillSound();
		}

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

					if (character->IsSelectedTalent[0][2])
					{
						projectile->slowValue = collideSlowValue;
						projectile->slowTime = collideSlowTime;
					}
					else
					{
						projectile->slowValue = static_cast<int>(0.f);
						projectile->slowTime = static_cast<int>(0.f);

					}

					isProjectileHeroHit = projectile->isHerohit;
				}

				projectile->FinishSpawning(SpawnTransform);
			}

			if (character->IsSelectedTalent[5][2])
			{
				character->AddAttackDamage("QSkill", addAttackDamage);
				BuffDuration = MaxDuration;
			}				
		}
	
	}
}

void UFairyWingESkill::SetCooldown()
{
	if (isProjectileHeroHit == true && character->IsSelectedTalent[6][2])
		skillCooldown = 3.f;
	else
		skillCooldown = skillMaxCooldown;
}

bool UFairyWingESkill::CanUse()
{
	if (!bIsTargeting)
	{
		return true;
	}

	auto SRController = Cast<ASidheRigelPlayerController>(character->GetController());
	if (SRController)
	{
		auto hit = SRController->GetHitResult();

		if (character->GetDistanceTo(hit.GetActor()) > range)
			return false;

		auto teamActor = Cast<ITeam>(hit.GetActor());
		if (teamActor && teamActor->GetTeam() != character->GetTeam())
			return true;

		if (teamActor && teamActor->GetTeam() == character->GetTeam() && character->IsSelectedTalent[4][1])
			return true;

	}

	return false;
}

void UFairyWingESkill::OnTick()
{
	Super::OnTick();

	if (character->IsSelectedTalent[5][2])
	{
		if (CheckAttackCount())
		{
			QuitESkill();
		}

		if (BuffDuration > 0)
		{
			BuffDuration -= 0.1f;

			if (BuffDuration <= 0)
			{
				QuitESkill();
			}
		}
	}	
}

void UFairyWingESkill::QuitESkill()
{
	character->RemoveAttackDamage("ESkill");
	isWorking = false;
	AttackCount = 0;
}

bool UFairyWingESkill::CheckAttackCount()
{
	return false;
}

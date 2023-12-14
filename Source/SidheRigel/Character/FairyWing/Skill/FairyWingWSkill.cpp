// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingWSkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/FairyWing/FairyWingCharacter.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingWCollider.h"

UFairyWingWSkill::UFairyWingWSkill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> colliderRef(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingWCollider"));
	if (colliderRef.Object)
	{
		colliderClass = (UClass*)colliderRef.Object->GeneratedClass;
	}
}

UFairyWingWSkill::~UFairyWingWSkill()
{
}

void UFairyWingWSkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 0.2f;
	skillCooldown = 0;
	skillMaxCooldown = 12.f;
	range = 700.f;
	requireMana = 70.f;

	bIsInstantCast = false;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UFairyWingWSkill::OnUse(FHitResult Hit)
{
	USkill::OnUse(Hit);

	FVector PawnToTarget = (Hit.Location - character->GetActorLocation()).GetSafeNormal();
	FRotator MuzzleRotation = PawnToTarget.Rotation();

	UWorld* World = character->GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Hit.Location);
		SpawnTransform.SetRotation(MuzzleRotation.Quaternion());
		SpawnParams.Owner = character;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile at the muzzle.
		AFairyWingWCollider* collider = World->SpawnActorDeferred<AFairyWingWCollider>(colliderClass, SpawnTransform);
		if (collider)
		{
			if (AFairyWingCharacter* FairyWingCharacter = Cast<AFairyWingCharacter>(character))
			{
				FairyWingCharacter->PlayWSkillMontage();
				FairyWingCharacter->PlayWSkillSound();
			}

			collider->colliderOwner = character;
			collider->duration = colliderDuration;
			collider->slowValue = colliderSlow;

			if (character->IsSelectedTalent[0][1])
				collider->damage = upgradeColliderDamage;
			else
				collider->damage = colliderDamage;

			if (character->IsSelectedTalent[6][1])
			{
				collider->slowValue += addColliderSlow;
				collider->damage += addColliderDamage;
			}				
		}

		collider->FinishSpawning(SpawnTransform);
	}
}
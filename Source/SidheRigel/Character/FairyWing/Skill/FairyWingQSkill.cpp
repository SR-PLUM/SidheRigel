// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingQSkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingQCollider.h"

UFairyWingQSkill::UFairyWingQSkill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> colliderRef(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingQCollider"));
	if (colliderRef.Object)
	{
		colliderClass = (UClass*)colliderRef.Object->GeneratedClass;
	}
}

UFairyWingQSkill::~UFairyWingQSkill()
{
}

void UFairyWingQSkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 1.f;
	range = 500.f;

	bIsInstantCast = false;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;
}

void UFairyWingQSkill::OnUse(FHitResult Hit)
{
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
		AFairyWingQCollider* collider = World->SpawnActorDeferred<AFairyWingQCollider>(colliderClass, SpawnTransform);
		if (collider)
		{
			collider->colliderOwner = character;
			collider->duration = colliderDuration;
			collider->damage = colliderDamage;
			collider->force = colliderForce;
		}

		collider->FinishSpawning(SpawnTransform);
	}
}

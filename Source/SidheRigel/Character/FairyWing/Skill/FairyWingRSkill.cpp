// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingRSkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingRCollider.h"


UFairyWingRSkill::UFairyWingRSkill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> colliderRef(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingRCollider"));
	if (colliderRef.Object)
	{
		colliderClass = (UClass*)colliderRef.Object->GeneratedClass;
	}
}

UFairyWingRSkill::~UFairyWingRSkill()
{
}

void UFairyWingRSkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 1.f;
	range = 500.f;

	bIsInstantCast = false;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UFairyWingRSkill::OnUse(FHitResult Hit)
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
		AFairyWingRCollider* collider = World->SpawnActorDeferred<AFairyWingRCollider>(colliderClass, SpawnTransform);
		if (collider)
		{
			collider->colliderOwner = character;
			collider->duration = colliderDuration;
			collider->damage = colliderDamage;
			collider->force = colliderForce;

			if (character->IsSelectedTalent[3][0])
				collider->silenceTime = colliderSilenceTime;
			else
				collider->silenceTime = static_cast<int>(0.f);
		}

		collider->FinishSpawning(SpawnTransform);
	}
}
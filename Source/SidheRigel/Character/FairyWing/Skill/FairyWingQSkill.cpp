// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingQSkill.h"
#include "../FairyWingCharacter.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingQCollider.h"
#include "Components/SphereComponent.h"

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
	skillMaxCooldown = 7.f;
	range = 700.f;
	requireMana = 70.f;

	bIsInstantCast = false;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UFairyWingQSkill::OnUse(FHitResult Hit)
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
		AFairyWingQCollider* collider = World->SpawnActorDeferred<AFairyWingQCollider>(colliderClass, SpawnTransform);
		if (collider)
		{
			if (AFairyWingCharacter* FairyWingCharacter = Cast<AFairyWingCharacter>(character))
			{
				UE_LOG(LogTemp, Warning, TEXT("UseQSkill2"));
				FairyWingCharacter->PlayQSkillMontage();
				FairyWingCharacter->PlayQSkillSound();
			}

			if (character->IsSelectedTalent[4][2])
			{
				collider->CollisionComponent->SetRelativeScale3D(FVector(2, 2, 2));
			}

			collider->colliderOwner = character;
			collider->force = colliderForce;

			if (character->IsSelectedTalent[0][0])
				collider->duration = upgradeColliderDuration;
			else
				collider->duration = colliderDuration;

			if (character->IsSelectedTalent[1][2])
				collider->damage = upgradeColliderDamage;
			else
				collider->damage = colliderDamage;			

			if(character->IsSelectedTalent[1][0])
				collider->restoreHPValue = colliderRestoreHPValue;
			else
				collider->restoreHPValue = static_cast<int>(0.f);

			if (character->IsSelectedTalent[1][1])
				collider->blindTime = colliderBlindTime;
			else
				collider->blindTime = static_cast<int>(0.f);

			if (character->IsSelectedTalent[5][1])
				collider->increaseAttackSpeed = increaseColliderAttackSpeed;
			else
				collider->increaseAttackSpeed = static_cast<int>(0.f);
		}

		collider->FinishSpawning(SpawnTransform);
	}
}

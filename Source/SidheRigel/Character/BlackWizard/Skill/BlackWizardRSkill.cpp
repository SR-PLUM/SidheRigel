// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackWizardRSkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardRCollider.h"

BlackWizardRSkill::BlackWizardRSkill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> colliderRef(TEXT("/Game/Heros/BlackWizard/Skill/BlackWizardRCollider"));
	if (colliderRef.Object)
	{
		colliderClass = (UClass*)colliderRef.Object->GeneratedClass;
	}
}

BlackWizardRSkill::~BlackWizardRSkill()
{
}

void BlackWizardRSkill::SetSkillProperty(ASidheRigelCharacter* Character)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 10.f;
	bIsInstantCast = false;

	character = Character;
}

void BlackWizardRSkill::OnUse(FHitResult Hit)
{
	FVector PawnToTarget = (Hit.Location - character->GetActorLocation()).GetSafeNormal();
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
		ABlackWizardRCollider* collider = World->SpawnActorDeferred<ABlackWizardQCollider>(colliderClass, SpawnTransform);
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
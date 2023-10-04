// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackWizardQSkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardQCollider.h"

BlackWizardQSkill::BlackWizardQSkill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> colliderRef(TEXT("/Game/Heros/BlackWizard/Skill/BlackWizardQCollider"));
	if (colliderRef.Object)
	{
		colliderClass = (UClass*)colliderRef.Object->GeneratedClass;
	}
}

BlackWizardQSkill::~BlackWizardQSkill()
{
}

void BlackWizardQSkill::SetSkillProperty(ASidheRigelCharacter* Character)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 10.f;
	bIsInstantCast = false;

	character = Character;
}

void BlackWizardQSkill::OnUse(FHitResult Hit)
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
		ABlackWizardQCollider* collider = World->SpawnActorDeferred<ABlackWizardQCollider>(colliderClass, SpawnTransform);
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

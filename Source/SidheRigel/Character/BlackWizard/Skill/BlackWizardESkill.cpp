// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackWizardESkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/BlackWizard/Skill/BlackWizardECollider.h"

UBlackWizardESkill::UBlackWizardESkill()
{
	static ConstructorHelpers::FObjectFinder<UClass> colliderRef(TEXT("Blueprint'/Game/Heros/BlackWizard/Skill/BP_BlackWizardECollider.BP_BlackWizardECollider_C'"));
	if (colliderRef.Object)
	{
		colliderClass = (UClass*)colliderRef.Object;
	}
}

UBlackWizardESkill::~UBlackWizardESkill()
{
}

void UBlackWizardESkill::SetSkillProperty(class ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 2.f;
	range = 500.f;

	bIsInstantCast = false;
	bIsTargeting = true;

	character = Character;
	skillstate = SkillState;

	character->GetWorldTimerManager().SetTimer(cooldownTimer, this, &USkill::OnTick, 0.1f, true);
}

void UBlackWizardESkill::OnUse(FHitResult Hit)
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
		ABlackWizardECollider* collider = World->SpawnActorDeferred<ABlackWizardECollider>(colliderClass, SpawnTransform);
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
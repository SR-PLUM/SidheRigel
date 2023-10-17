// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdESkill.h"

#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Cold/Skill/ColdEProjectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdEDamageField.h"

ColdESkill::ColdESkill()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> projectileRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdEProjectile"));
	if (projectileRef.Object)
	{
		projectileClass = (UClass*)projectileRef.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> damageFieldRef(TEXT("/Game/Heros/Cold/Skill/BP_ColdEDamageField"));
	if (damageFieldRef.Object)
	{
		damageFieldClass = (UClass*)damageFieldRef.Object->GeneratedClass;
	}
}

ColdESkill::~ColdESkill()
{
}

void ColdESkill::SetSkillProperty(ASidheRigelCharacter* Character, E_SkillState SkillState)
{
	skillDelay = 1.f;
	skillCooldown = 0;
	skillMaxCooldown = 3.f;
	range = 500.f;

	bIsInstantCast = true;
	bIsTargeting = false;

	character = Character;
	skillstate = SkillState;
}

void ColdESkill::OnUse(FHitResult Hit)
{
	FVector MuzzleLocation = character->GetActorLocation() + FVector::UpVector * 50;
	FRotator MuzzleRotation = character->GetActorRotation();
	
	UWorld* World = character->GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = character;
		SpawnParams.Instigator = character->GetInstigator();
	
		// Spawn the projectile at the muzzle.
		AColdEProjectile* projectile = World->SpawnActor<AColdEProjectile>(projectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (projectile)
		{
			projectile->projectileOwner = character;
			projectile->Launch(MuzzleLocation, Hit.Location);
		}

		FActorSpawnParameters FieldSpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Hit.Location * FVector(1, 1, 0));
		SpawnTransform.SetRotation(MuzzleRotation.Quaternion());
		FieldSpawnParams.Owner = character;
		FieldSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile at the muzzle.
		projectile->damageField = World->SpawnActorDeferred<AColdEDamageField>(damageFieldClass, SpawnTransform);
		if (projectile->damageField)
		{
			projectile->damageField->projectileOwner = character;
			projectile->damageField->fieldArea = colliderFieldArea;
			projectile->damageField->damage = colliderDamage;
		}

		projectile->damageField->FinishSpawning(SpawnTransform);
	}
}

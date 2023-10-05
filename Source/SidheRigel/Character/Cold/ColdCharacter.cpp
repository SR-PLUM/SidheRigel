// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "SidheRigel/Character/Cold/Skill/ColdQSkill.h"
#include "SidheRigel/Character/Cold/Skill/ColdWSkill.h"
#include "SidheRigel/Character/Cold/Skill/ColdESkill.h"
#include "SidheRigel/Character/Cold/Skill/ColdR1Skill.h"
#include "SidheRigel/Character/Cold/Skill/ColdWProjectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdEProjectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdEDamageField.h"
#include "SidheRigel/Character/Cold/Skill/ColdR1Projectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdR2Projectile.h"
#include "SidheRigel/Character/Cold/ColdAttackProjectile.h"

// Sets default values
AColdCharacter::AColdCharacter()
{
	ultType = E_UltType::Ult1;

	static ConstructorHelpers::FObjectFinder<UBlueprint> R1Projectile(TEXT("/Game/Heros/Cold/Skill/BP_ColdR1Projectile"));
	if (R1Projectile.Object)
	{
		R1ProjectileClass = (UClass*)R1Projectile.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> R2Projectile(TEXT("/Game/Heros/Cold/Skill/BP_ColdR2Projectile"));
	if (R2Projectile.Object)
	{
		R2ProjectileClass = (UClass*)R2Projectile.Object->GeneratedClass;
	}

	InitAttackProjectile();

	skills.Add({ E_SkillState::Q_Ready, new ColdQSkill });
	if (skills[E_SkillState::Q_Ready] != nullptr)
	{
		skills[E_SkillState::Q_Ready]->SetSkillProperty(this, E_SkillState::Q_Ready);
	}
	skills.Add({ E_SkillState::W_Ready, new ColdWSkill });
	if (skills[E_SkillState::W_Ready] != nullptr)
	{
		skills[E_SkillState::W_Ready]->SetSkillProperty(this, E_SkillState::W_Ready);
	}
	skills.Add({ E_SkillState::E_Ready, new ColdESkill });
	if (skills[E_SkillState::E_Ready] != nullptr)
	{
		skills[E_SkillState::E_Ready]->SetSkillProperty(this, E_SkillState::E_Ready);
	}
	skills.Add({ E_SkillState::R_Ready, new ColdR1Skill });
	if (skills[E_SkillState::R_Ready] != nullptr)
	{
		skills[E_SkillState::R_Ready]->SetSkillProperty(this, E_SkillState::R_Ready);
	}
}

// Called when the game starts or when spawned
void AColdCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AColdCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AColdCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AColdCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/Cold/BP_ColdAttackProjectile"));
	if (Projectile.Object)
	{
		attackProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void AColdCharacter::Attack(AActor* target)
{
	if (attackProjectileClass)
	{
		FVector MuzzleLocation = GetActorLocation();
		FRotator MuzzleRotation = GetActorRotation();

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			AColdAttackProjectile* Projectile = World->SpawnActor<AColdAttackProjectile>(attackProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->Target = target;
				InitProjectileProperty(Projectile);
			}
		}
	}
}

//void AColdCharacter::EImplement(FHitResult HitResult)
//{
//	
//}
//
//void AColdCharacter::RImplement(FHitResult HitResult)
//{
//	switch (ultType)
//	{
//	case None:
//		break;
//	case Ult1:
//		R1Implement(HitResult);
//		break;
//	case Ult2:
//		R2Implement(HitResult);
//		break;
//	default:
//		break;
//	}
//}
//
//void AColdCharacter::R1Implement(FHitResult HitResult)
//{
//	
//}
//
//void AColdCharacter::R2Implement(FHitResult HitResult)
//{

//}
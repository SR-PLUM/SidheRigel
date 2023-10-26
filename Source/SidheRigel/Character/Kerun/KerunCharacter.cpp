// Fill out your copyright notice in the Description page of Project Settings.

#include "KerunCharacter.h"
#include "../../Dummy/DummyProjectile.h"

#include "Skills/KerunQSkill.h"
#include "Skills/KerunWSkill.h"
#include "Skills/KerunR1Skill.h"
#include "Skills/KerunR2Skill.h"

#include "KerunAttackProjectile.h"
#include "KerunAnimInstance.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AKerunCharacter::AKerunCharacter()
{
	InitAttackProjectile();

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	
	
}

void AKerunCharacter::BeginPlay()
{
	Super::BeginPlay();

	KerunWSkillRef = NewObject<UKerunWSkill>();
	KerunR1SkillRef = NewObject<UKerunR1Skill>();
	KerunR2SkillRef = NewObject<UKerunR2Skill>();

	//AnimInstance = Cast<UKerunAnimInstance>(GetMesh()->GetAnimInstance());
	
	skills.Add({ E_SkillState::Q_Ready, NewObject<UKerunQSkill>() });
	if (skills[E_SkillState::Q_Ready] != nullptr)
	{
		skills[E_SkillState::Q_Ready]->SetSkillProperty(this, E_SkillState::Q_Ready);
	}
	/*
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
	*/

}

void AKerunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Kerun QSkill
	UKerunQSkill* QSkillRef = Cast<UKerunQSkill>(skills[E_SkillState::Q_Ready]);

	if (QSkillRef->CheckAttackCount())
	{
		QSkillRef->QuitQSkill();
	}

	//Kerun WSkill
	if (KerunWSkillRef->GetIsWorking())
	{
		FVector Loc = GetActorLocation();

		if (Loc.Z >= KerunWSkillRef->GetLimitZValue())
		{
			KerunWSkillRef->KnockDownTarget(this);
		}
		
	}
}

void AKerunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AKerunCharacter::InitProperty()
{
	level = 1;
	range.Add("Debug", 200.f);
	attackDamage.Add("Debug", 5.f);
	attackSpeed.Add("Debug", 1.f);
	criticalRate.Add("Debug", 50);
	criticalDamage.Add("Debug", 50);

	MaxHP.Add("Debug", 100.f);
	generateHealthPoint.Add("Debug", 0.2f);
	MaxMP.Add("Debug", 100.f);
	lifeSteal.Add("Debug", 5.f);
	protectPower.Add("Debug", 20);

	defencePoint.Add("Debug", 100);

	speed.Add("Debug", 600.f);

	currentHP = GetMaxHP();
	currentMP = GetMaxMP();

	MaxExperience = 20;
}

void AKerunCharacter::Attack(AActor* target)
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
		AKerunAttackProjectile* Projectile = World->SpawnActor<AKerunAttackProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			// Set the projectile's initial trajectory.
			Projectile->Target = target;
			InitProjectileProperty(Projectile);

			//Kerun QSkill
			UKerunQSkill* QSkillRef = Cast<UKerunQSkill>(skills[E_SkillState::Q_Ready]);

			if (QSkillRef->IsWorking)
			{
				QSkillRef->AttackCount += 1;
			}

			ImproveEStack(1);
			
		}
	}

	/*
	if (AnimInstance)
	{
		AnimInstance->PlayAttackMontage();
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance is Not Null"));
	}
	*/
}

void AKerunCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/Kerun/BP_KerunAttackProjectile"));
	if (Projectile.Object)
	{
		ProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void AKerunCharacter::SetCurrentHP(float _hp)
{
	Super::SetCurrentHP(_hp);

	if (KerunR2SkillRef->GetIsWorking())
	{
		if (KerunR2SkillRef->CheckCurrentHP(currentHP))
		{
			currentHP = KerunR2SkillRef->GetMinHP();
		}
	}
}

//TODO::REFECTOR THIS

void AKerunCharacter::UseSkill(FHitResult HitResult, E_SkillState SkillState)
{
	switch (SkillState)
	{
	case Skill_Null:
		UE_LOG(LogTemp, Warning, TEXT("skillState is Null"));
		break;
	
	case W_Ready:
		UE_LOG(LogTemp, Warning, TEXT("skillState is W"));

		if (AActor* Actor = HitResult.GetActor())
		{
			if (Actor->Tags.Contains("Hero"))
			{
				KerunWSkillRef->JumpIntoTarget(Actor, this);

				ImproveEStack(3);
			}
		}

		break;

	case R_Ready:
		UE_LOG(LogTemp, Warning, TEXT("skillState is R"));

		if (AActor* Actor = HitResult.GetActor())
		{
			if (Actor->Tags.Contains("Hero"))
			{
				KerunR1SkillRef->StunTarget(Actor, this);

				ImproveEStack(6);
			}
		}
		break;
	}
}

void AKerunCharacter::ImproveEStack(int Count)
{

	ECurrentStack += Count;

	if (ECurrentStack > EMaxStack)
	{
		ECurrentStack = EMaxStack;
	}

	StartETimer();
}

void AKerunCharacter::StartETimer()
{
	QuitETimer();

	float GenerateAmount = ECurrentStack * EHealthRate;

	generateHealthPoint.Add("ESkill", GenerateAmount);

	GetWorld()->GetTimerManager().SetTimer(
		ETimer,
		FTimerDelegate::CreateLambda([&]() {
			QuitETimer();
			ECurrentStack = 0;
			UE_LOG(LogTemp, Warning, TEXT("ECurrentStack Initialized"));
			}),
		EDuration,
		false
		);
}

void AKerunCharacter::QuitETimer()
{
	GetWorldTimerManager().ClearTimer(ETimer);
	generateHealthPoint.Remove("ESkill");
}

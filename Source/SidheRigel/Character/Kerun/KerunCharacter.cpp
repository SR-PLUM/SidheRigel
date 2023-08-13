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

	KerunQSkillRef = NewObject<UKerunQSkill>();
	KerunWSkillRef = NewObject<UKerunWSkill>();
	KerunR1SkillRef = NewObject<UKerunR1Skill>();
	KerunR2SkillRef = NewObject<UKerunR2Skill>();

	//AnimInstance = Cast<UKerunAnimInstance>(GetMesh()->GetAnimInstance());
	
}

void AKerunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Kerun QSkill
	if (KerunQSkillRef->CheckAttackCount())
	{
		KerunQSkillRef->QuitQSkill(attackSpeed);
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
	range.Add("Debug", 200.f);
	attackDamage.Add("Debug", 5.f);
	attackSpeed.Add("Debug", 1.f);
	criticalRate.Add("Debug", 50);
	criticalDamage.Add("Debug", 50);
	MaxHP.Add("Debug", 100.f);
	generateHealthPoint.Add("Debug", 0.2f);
	lifeSteal.Add("Debug", 5.f);
	protectPower.Add("Debug", 20);

	currentHP = GetMaxHP();
}

void AKerunCharacter::SpawnAttackProjectile()
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
			InitProjectileProperty(Projectile);

			//Kerun QSkill
			if (KerunQSkillRef->IsWorking)
			{
				KerunQSkillRef->AttackCount += 1;
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

void AKerunCharacter::SkillOne()
{
	KerunQSkillRef->ImproveAttackSpeed(attackSpeed, this);
}

void AKerunCharacter::SkillTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("Kerun WSkill"));

	skillState = W_Ready;
}

void AKerunCharacter::SkillFour()
{
	//R1 Skill

	UE_LOG(LogTemp, Warning, TEXT("Kerun R1Skill"));

	skillState = R_Ready;


	//R2 Skill
	/*UE_LOG(LogTemp, Warning, TEXT("Kerun R2Skill"));
	KerunR2SkillRef->StartR2Buff(this);*/
}

void AKerunCharacter::SkillCancel()
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Cancel"));

	skillState = Null;
}

void AKerunCharacter::UseSkill(FHitResult HitResult)
{
	switch (skillState)
	{
	case Null:
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

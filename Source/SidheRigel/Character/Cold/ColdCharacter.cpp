// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "SidheRigel/Character/Cold/Skill/ColdQProjectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdWProjectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdR1Projectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdR2Projectile.h"
#include "SidheRigel/Character/Cold/ColdAttackProjectile.h"

// Sets default values
AColdCharacter::AColdCharacter()
{
	skillState = E_SkillState::Null;
	ultType = E_UltType::Ult2;

	static ConstructorHelpers::FObjectFinder<UBlueprint> QProjectile(TEXT("/Game/Heros/Cold/Skill/BP_ColdQProjectile"));
	if (QProjectile.Object)
	{
		QProjectileClass = (UClass*)QProjectile.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> WProjectile(TEXT("/Game/Heros/Cold/Skill/BP_ColdWProjectile"));
	if (WProjectile.Object)
	{
		WProjectileClass = (UClass*)WProjectile.Object->GeneratedClass;
	}

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

void AColdCharacter::SpawnAttackProjectile()
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
				InitProjectileProperty(Projectile);
			}
		}
	}
}

void AColdCharacter::SkillOne()
{
	UE_LOG(LogTemp, Warning, TEXT("Cold Ready Q"));

	skillState = Q_Ready;
}

void AColdCharacter::QImplement(FHitResult HitResult)
{
	if (AActor* _target = HitResult.GetActor())
	{
		if (_target->Tags.Contains("Hero"))
		{
			if (QProjectileClass)
			{
				for (int32 i = 0; i < QCount; i++)
				{
					FTimerHandle QProjectileGenerateTimer;
					GetWorldTimerManager().SetTimer(QProjectileGenerateTimer,
						FTimerDelegate::CreateLambda([=]()
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
									AColdQProjectile* Projectile = World->SpawnActor<AColdQProjectile>(QProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
									if (Projectile)
									{
										Projectile->Target = _target;
										Projectile->projectileOwner = this;
									}
								}
							}), (float)i * 0.1f, false);
				}
			}
		}
	}
}

void AColdCharacter::SkillTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("Cold Ready W"));

	skillState = W_Ready;
}

void AColdCharacter::WImplement(FHitResult HitResult)
{
	FVector PawnToTarget = (HitResult.Location - GetActorLocation()).GetSafeNormal();
	FVector MuzzleLocation = GetActorLocation() + PawnToTarget*50;
	FRotator MuzzleRotation = PawnToTarget.Rotation();

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(MuzzleLocation);
		SpawnTransform.SetRotation(MuzzleRotation.Quaternion());
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile at the muzzle.
		AColdWProjectile* Projectile = World->SpawnActorDeferred<AColdWProjectile>(WProjectileClass, SpawnTransform);
		if (Projectile)
		{
			Projectile->projectileOwner = this;
		}

		Projectile->FinishSpawning(SpawnTransform);
	}
}

void AColdCharacter::SkillThree()
{
	UE_LOG(LogTemp, Warning, TEXT("Cold Ready E"));

	skillState = E_Ready;
}

void AColdCharacter::SkillFour()
{
	UE_LOG(LogTemp, Warning, TEXT("Cold Ready R"));

	GetCameraBoom()->TargetArmLength = 1600.f;
	skillState = R_Ready;
}

void AColdCharacter::RImplement(FHitResult HitResult)
{
	switch (ultType)
	{
	case None:
		break;
	case Ult1:
		R1Implement(HitResult);
		break;
	case Ult2:
		R2Implement(HitResult);
		break;
	default:
		break;
	}
}

void AColdCharacter::R1Implement(FHitResult HitResult)
{
	if (AActor* _target = HitResult.GetActor())
	{
		if (_target->Tags.Contains("Hero"))
		{
			if (R1ProjectileClass)
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
					AColdR1Projectile* Projectile = World->SpawnActor<AColdR1Projectile>(R1ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						Projectile->Target = _target;
						Projectile->projectileOwner = this;
					}
				}
			}
		}
	}
}

void AColdCharacter::R2Implement(FHitResult HitResult)
{
	FVector PawnToTarget = (HitResult.Location - GetActorLocation()).GetSafeNormal();
	FVector MuzzleLocation = GetActorLocation() + PawnToTarget * 50;
	FRotator MuzzleRotation = PawnToTarget.Rotation();

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(MuzzleLocation);
		SpawnTransform.SetRotation(MuzzleRotation.Quaternion());
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile at the muzzle.
		AColdR2Projectile* Projectile = World->SpawnActorDeferred<AColdR2Projectile>(R2ProjectileClass, SpawnTransform);
		if (Projectile)
		{
			Projectile->projectileOwner = this;
			Projectile->startLocation = MuzzleLocation;
			Projectile->forwardVector = PawnToTarget;
		}

		Projectile->FinishSpawning(SpawnTransform);
	}
}



void AColdCharacter::SkillCancel()
{
	UE_LOG(LogTemp, Warning, TEXT("Cold SkillCancel"));

	if (skillState == R_Ready)
	{
		GetCameraBoom()->TargetArmLength = 800.f;
	}
	skillState = Null;
}

void AColdCharacter::UseSkill(FHitResult HitResult)
{
	switch (skillState)
	{
	case Null:
		UE_LOG(LogTemp, Warning, TEXT("Cold SkillState is Null"));
		break;
	case Q_Ready:
		UE_LOG(LogTemp, Warning, TEXT("Cold use Q"));

		QImplement(HitResult);
		skillState = Null;
		break;
	case W_Ready:
		UE_LOG(LogTemp, Warning, TEXT("Cold use W"));

		WImplement(HitResult);
		skillState = Null;
		break;
	case E_Ready:
		UE_LOG(LogTemp, Warning, TEXT("Cold use E"));
		skillState = Null;
		break;
	case R_Ready:
		UE_LOG(LogTemp, Warning, TEXT("Cold use R"));

		RImplement(HitResult);
		skillState = Null;

		GetCameraBoom()->TargetArmLength = 800.f;
		break;
	default:
		break;
	}
}


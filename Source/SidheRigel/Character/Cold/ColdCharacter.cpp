// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdCharacter.h"
#include "SidheRigel/Character/Cold/Skill/ColdQProjectile.h"
#include "SidheRigel/Character/Cold/Skill/ColdWProjectile.h"
#include "SidheRigel/Character/Cold/ColdAttackProjectile.h"

// Sets default values
AColdCharacter::AColdCharacter()
{
	skillState = Null;

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
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn the projectile at the muzzle.
		AColdWProjectile* Projectile = World->SpawnActor<AColdWProjectile>(WProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			Projectile->setProjectileOwner(this);
		}
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

	skillState = R_Ready;
}

void AColdCharacter::SkillCancel()
{
	UE_LOG(LogTemp, Warning, TEXT("Cold SkillCancel"));

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
		skillState = Null;
		break;
	default:
		break;
	}
}


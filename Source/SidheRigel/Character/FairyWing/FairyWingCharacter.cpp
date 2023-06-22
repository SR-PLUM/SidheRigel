// Fill out your copyright notice in the Description page of Project Settings.


#include "FairyWingCharacter.h"
#include "Components/SphereComponent.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingQCollider.h"
#include "SidheRigel/Character/FairyWing/Skill/FairyWingEProjectile.h"
#include "SidheRigel/Character/FairyWing/FairyWingAttackProjectile.h"

// Sets default values
AFairyWingCharacter::AFairyWingCharacter()
{
 	skillState = Null;

	static ConstructorHelpers::FObjectFinder<UBlueprint> EProjectile(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingEProjectile"));
	if (EProjectile.Object)
	{
		EProjectileClass = (UClass*)EProjectile.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> QCollider(TEXT("/Game/Heros/FairyWing/Skill/BP_FairyWingQCollider"));
	if (QCollider.Object)
	{
		QColliderClass = (UClass*)QCollider.Object->GeneratedClass;
	}

	InitAttackProjectile();
}

// Called when the game starts or when spawned
void AFairyWingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFairyWingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFairyWingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFairyWingCharacter::InitProperty()
{
	range.Add("Debug", 1000.f);
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

void AFairyWingCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/FairyWing/BP_FairyWingAttackProjectile"));
	if (Projectile.Object)
	{
		attackProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void AFairyWingCharacter::SpawnAttackProjectile()
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
			AFairyWingAttackProjectile* Projectile = World->SpawnActor<AFairyWingAttackProjectile>(attackProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				InitProjectileProperty(Projectile);
			}
		}
	}
}

void AFairyWingCharacter::SkillOne()
{
	UE_LOG(LogTemp, Warning, TEXT("FairyWing Ready Q"));

	skillState = Q_Ready;
}

void AFairyWingCharacter::SkillTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("FairyWing Ready W"));

	skillState = W_Ready;
}

void AFairyWingCharacter::SkillThree()
{
	UE_LOG(LogTemp, Warning, TEXT("FairyWing Ready E"));

	skillState = E_Ready;
}

void AFairyWingCharacter::SkillFour()
{
	UE_LOG(LogTemp, Warning, TEXT("FairyWing Ready R"));

	skillState = R_Ready;
}

void AFairyWingCharacter::SkillCancel()
{
	UE_LOG(LogTemp, Warning, TEXT("FairyWing SkillCancel"));

	skillState = Null;
}

void AFairyWingCharacter::UseSkill(FHitResult HitResult)
{
	switch (skillState)
	{
	case Null:
		UE_LOG(LogTemp, Warning, TEXT("FairyWing SkillState is Null"));
		break;
	case Q_Ready:
		UE_LOG(LogTemp, Warning, TEXT("FairyWing use Q"));

		if (QColliderClass)
		{
			FVector MuzzleLocation = HitResult.Location;
			FRotator MuzzleRotation = GetActorRotation();

			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				// Spawn the projectile at the muzzle.
				AFairyWingQCollider* Collider = World->SpawnActor<AFairyWingQCollider>(QColliderClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (Collider)
				{
					// Set the projectile's initial trajectory.
					Collider->colliderOwner = this;

					FTimerHandle QColliderDestroyTimer;
					GetWorldTimerManager().SetTimer(QColliderDestroyTimer,
						FTimerDelegate::CreateLambda([=]()
							{
								Collider->Destroy();
							}

					), 1.0f, false);

					Collider->CollisionComponent->SetGenerateOverlapEvents(false);
				}
			}
		}

		skillState = Null;
		break;
	case W_Ready:
		UE_LOG(LogTemp, Warning, TEXT("FairyWing use W"));
		skillState = Null;
		break;
	case E_Ready:
		UE_LOG(LogTemp, Warning, TEXT("FairyWing use E"));

		if (AActor* _target = HitResult.GetActor())
		{
			if (_target->Tags.Contains("Hero"))
			{
				if (EProjectileClass)
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
						AFairyWingEProjectile* Projectile = World->SpawnActor<AFairyWingEProjectile>(EProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
						if (Projectile)
						{
							// Set the projectile's initial trajectory.
							Projectile->Target = _target;
							Projectile->projectileOwner = this;
						}
					}
				}
			}
		}
		

		skillState = Null;
		break;
	case R_Ready:
		UE_LOG(LogTemp, Warning, TEXT("FairyWing use R"));
		skillState = Null;
		break;
	default:
		break;
	}
}
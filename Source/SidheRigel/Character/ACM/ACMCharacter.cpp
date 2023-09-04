// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMCharacter.h"
#include "ACMAttackProjectile.h"

//Skill Collider
#include "Skills/ACMQCollider.h"
#include "Skills/ACMECollider.h"

AACMCharacter::AACMCharacter()
{
	InitAttackProjectile();
	
	InitColliderPath();
}

void AACMCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AACMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (QCoolDownRemain > 0.f)
	{
		QCoolDownRemain -= DeltaTime;
		if (QCoolDownRemain < 0.f)
		{
			QCoolDownRemain = 0.f;
		}
	}

	if (WCoolDownRemain > 0.f)
	{
		WCoolDownRemain -= DeltaTime;
		if (WCoolDownRemain < 0.f)
		{
			WCoolDownRemain = 0.f;
		}
	}

	if (ECoolDownRemain > 0.f)
	{
		ECoolDownRemain -= DeltaTime;
		if (ECoolDownRemain < 0.f)
		{
			ECoolDownRemain = 0.f;
		}
	}

	if (R1CoolDownRemain > 0.f)
	{
		R1CoolDownRemain -= DeltaTime;
		if (R1CoolDownRemain < 0.f)
		{
			R1CoolDownRemain = 0.f;
		}
	}

	if (R2CoolDownRemain > 0.f)
	{
		R2CoolDownRemain -= DeltaTime;
		if (R2CoolDownRemain < 0.f)
		{
			R2CoolDownRemain = 0.f;
		}
	}

}

void AACMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AACMCharacter::InitProperty()
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

void AACMCharacter::InitColliderPath()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> QCollider(TEXT("/Game/Heros/ACM/Skills/BP_ACMQCollider"));
	if (QCollider.Object)
	{
		ACMQCollider = (UClass*)QCollider.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> ECollider(TEXT("/Game/Heros/ACM/Skills/BP_ACMECollider"));
	if (ECollider.Object)
	{
		ACMECollider = (UClass*)ECollider.Object->GeneratedClass;
	}
}

void AACMCharacter::SpawnAttackProjectile()
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
		AACMAttackProjectile* Projectile = World->SpawnActor<AACMAttackProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			// Set the projectile's initial trajectory.
			InitProjectileProperty(Projectile);

			ReduceCoolDownRemain();
		}
	}
}

void AACMCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Heros/ACM/BP_ACMAttackProjectile"));
	if (Projectile.Object)
	{
		ProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void AACMCharacter::SkillOne()
{
	UE_LOG(LogTemp, Warning, TEXT("ACM Ready Q"));

	skillState = Q_Ready;
}

void AACMCharacter::QSkill(FHitResult HitResult)
{
	if (QCoolDownRemain > 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACM Q is CoolDowning"));
	}
	else
	{
		QImplement(HitResult);
		QCoolDownRemain = QCoolDown;
	}
}

void AACMCharacter::QImplement(FHitResult HitResult)
{
	if (ACMQCollider)
	{
		FVector PawnToTarget = (HitResult.Location - GetActorLocation()).GetSafeNormal();
		PawnToTarget *= FVector(75, 75, 0);
		FVector MuzzleLocation = (GetActorLocation() + PawnToTarget) * FVector(1, 1, 0);
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
			//SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			AACMQCollider* Collider = World->SpawnActorDeferred<AACMQCollider>(ACMQCollider, SpawnTransform);

			UE_LOG(LogTemp, Warning, TEXT("ACM QSkill Collider :: %s"), *(Collider->GetActorLabel()));
			if (Collider)
			{
				AddSkillCount();
				// Set the projectile's initial trajectory.
				Collider->ColliderOwner = this;

				Collider->IsUpgraded = UpgradeNextSkill;
				if (UpgradeNextSkill)
				{
					UE_LOG(LogTemp, Warning, TEXT("Skill Upgraded"));
					InitializeSkillCount();
				}

				FTimerHandle QColliderDestroyTimer;
				GetWorldTimerManager().SetTimer(QColliderDestroyTimer,
					FTimerDelegate::CreateLambda([=]()
						{
							Collider->Destroy();
						}

				), 1.0f, false);

				Collider->FinishSpawning(SpawnTransform);

				UE_LOG(LogTemp, Warning, TEXT("ACM QSkill Spawned"));

				Collider->MeshComponent->SetGenerateOverlapEvents(false);
			}
		}
	}
}

void AACMCharacter::SkillThree()
{
	UE_LOG(LogTemp, Warning, TEXT("ACM Ready E"));

	skillState = E_Ready;
}

void AACMCharacter::ESkill(FHitResult HitResult)
{
	if (ECoolDownRemain > 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACM E is CoolDowning"));
	}
	else
	{
		EImplement(HitResult);
		ECoolDownRemain = ECoolDown;
	}
}

void AACMCharacter::EImplement(FHitResult HitResult)
{
	if (ACMECollider)
	{
		FVector PawnToTarget = (HitResult.Location - GetActorLocation()).GetSafeNormal();
		PawnToTarget *= FVector(1, 1, 0);
		FVector MuzzleLocation = (GetActorLocation() + PawnToTarget) * FVector(1, 1, 0);
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
			//SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			AACMECollider* Collider = World->SpawnActorDeferred<AACMECollider>(ACMECollider, SpawnTransform);

			UE_LOG(LogTemp, Warning, TEXT("ACM ESkill Collider :: %s"), *(Collider->GetActorLabel()));
			if (Collider)
			{
				AddSkillCount();
				// Set the projectile's initial trajectory.
				Collider->ColliderOwner = this;

				Collider->IsUpgraded = UpgradeNextSkill;
				if (UpgradeNextSkill)
				{
					UE_LOG(LogTemp, Warning, TEXT("Skill Upgraded"));
					InitializeSkillCount();
				}

				FTimerHandle EColliderDestroyTimer;
				GetWorldTimerManager().SetTimer(EColliderDestroyTimer,
					FTimerDelegate::CreateLambda([=]()
						{
							Collider->Destroy();
						}

				), 1.0f, false);

				Collider->FinishSpawning(SpawnTransform);

				UE_LOG(LogTemp, Warning, TEXT("ACM ESkill Spawned"));

				Collider->MeshComponent->SetGenerateOverlapEvents(false);
			}
		}
	}
}

void AACMCharacter::SkillFour()
{
	UE_LOG(LogTemp, Warning, TEXT("ACM Ready R"));

	skillState = R_Ready;
}

void AACMCharacter::RSkill(FHitResult HitResult)
{
	//R1Skill
	R1Skill(HitResult);

	//R2Skill TODO
}

void AACMCharacter::R1Skill(FHitResult HitResult)
{
	if (R1CoolDownRemain > 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACM R1 is CoolDowning"));
	}
	else
	{
		switch (LastSkill)
		{
		case Null:
			UE_LOG(LogTemp, Warning, TEXT("ACM LastSkill is Null"));
			break;
		case Q_Ready:
			UE_LOG(LogTemp, Warning, TEXT("ACM R1Skill :: Q"));
			QImplement(HitResult);
			skillState = Null;
			R1CoolDownRemain = R1CoolDown;
			break;
		case W_Ready:
			UE_LOG(LogTemp, Warning, TEXT("ACM R1Skill :: W"));
			skillState = Null;
			R1CoolDownRemain = R1CoolDown;
			break;
		case E_Ready:
			UE_LOG(LogTemp, Warning, TEXT("ACM R1Skill :: E"));
			EImplement(HitResult);
			skillState = Null;
			R1CoolDownRemain = R1CoolDown;
			break;
		default:
			break;
		}
	}
}

void AACMCharacter::SkillCancel()
{
	UE_LOG(LogTemp, Warning, TEXT("ACM SkillCancel"));

	skillState = Null;
}

void AACMCharacter::UseSkill(FHitResult HitResult)
{
	switch (skillState)
	{
	case Null:
		UE_LOG(LogTemp, Warning, TEXT("ACM SkillState is Null"));
		break;
	case Q_Ready:
		UE_LOG(LogTemp, Warning, TEXT("ACM use Q"));
		QSkill(HitResult);
		skillState = Null;
		LastSkill = Q_Ready;
		break;
	case W_Ready:
		UE_LOG(LogTemp, Warning, TEXT("ACM use W"));
		skillState = Null;
		LastSkill = W_Ready;
		break;
	case E_Ready:
		UE_LOG(LogTemp, Warning, TEXT("ACM use E"));
		ESkill(HitResult);
		skillState = Null;
		LastSkill = E_Ready;
		break;
	case R_Ready:
		UE_LOG(LogTemp, Warning, TEXT("ACM use R"));
		RSkill(HitResult);
		skillState = Null;
		break;
	default:
		break;
	}
}

void AACMCharacter::AddSkillCount()
{
	SkillCount++;

	if (SkillCount == 1)
	{
		GetWorld()->GetTimerManager().SetTimer(
			UpgradingPassiveTimer,
			FTimerDelegate::CreateLambda([&]() {

				InitializeSkillCount();
				UE_LOG(LogTemp, Warning, TEXT("SkillCount Initialized"));
				}),
			CountDuration,
			false
			);
	}
	else if (SkillCount >=3)
	{
		UpgradeNextSkill = true;
	}
}

void AACMCharacter::InitializeSkillCount()
{
	SkillCount = 0;
	UpgradeNextSkill = false;

	GetWorld()->GetTimerManager().ClearTimer(UpgradingPassiveTimer);
	UE_LOG(LogTemp, Warning, TEXT("ClearTimer"));

}

void AACMCharacter::ReduceCoolDownRemain()
{
	QCoolDownRemain -= ReduceCoolDownAmount;
	WCoolDownRemain -= ReduceCoolDownAmount;
	ECoolDownRemain -= ReduceCoolDownAmount;

	if (QCoolDownRemain < 0.f) QCoolDownRemain = 0.f;
	if (WCoolDownRemain < 0.f) WCoolDownRemain = 0.f;
	if (ECoolDownRemain < 0.f) ECoolDownRemain = 0.f;
}

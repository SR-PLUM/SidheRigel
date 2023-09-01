﻿// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Dummy/DummyProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ASidheRigelCharacter::ASidheRigelCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	InitAttackProjectile();
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("DEBUG")));
}

void ASidheRigelCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitProperty();

	GetWorldTimerManager().SetTimer(GenerateHPTimer, this, &ASidheRigelCharacter::IE_GenerateHP, 1.f, true);
}

void ASidheRigelCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (IsMoveVectorTrue)
	{
		FVector Location = GetActorLocation();
		Location += moveDirection * (moveForce/10) * DeltaSeconds;
		SetActorLocation(Location);
		moveCnt++;
		if (moveCnt <= 10)
		{
			moveDirection = FVector::ZeroVector;
			moveForce = 0;
			moveCnt = 0;

			IsMoveVectorTrue = false;
		}
	}
}

void ASidheRigelCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("SkillOne", IE_Pressed, this, &ASidheRigelCharacter::SkillOne);
	PlayerInputComponent->BindAction("SkillTwo", IE_Pressed, this, &ASidheRigelCharacter::SkillTwo);
	PlayerInputComponent->BindAction("SkillThree", IE_Pressed, this, &ASidheRigelCharacter::SkillThree);
	PlayerInputComponent->BindAction("SkillFour", IE_Pressed, this, &ASidheRigelCharacter::SkillFour);
}

void ASidheRigelCharacter::SkillOne()
{
	UE_LOG(LogTemp, Warning, TEXT("Q"));
}

void ASidheRigelCharacter::SkillTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("W"));
}

void ASidheRigelCharacter::SkillThree()
{
	UE_LOG(LogTemp, Warning, TEXT("E"));
}

void ASidheRigelCharacter::SkillFour()
{
	UE_LOG(LogTemp, Warning, TEXT("R"));
}

void ASidheRigelCharacter::SkillCancel()
{
	UE_LOG(LogTemp, Warning, TEXT("Cancel"));
}

void ASidheRigelCharacter::UseSkill(FHitResult HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("UseSkill"));
}

void ASidheRigelCharacter::SetLevel(int32 _level)
{
	level = _level;

	if ((level != 19) && (level % 3 == 1 || level == 20))
	{
		//특성 띄우기
	}
}

void ASidheRigelCharacter::SetCurrentHP(float _hp)
{
	currentHP = _hp;

	float var_MaxHP = GetMaxHP();
	if (currentHP > var_MaxHP)
	{
		currentHP = var_MaxHP;
	}
}

float ASidheRigelCharacter::GetCurrentHP()
{
	return currentHP;
}

void ASidheRigelCharacter::IE_GenerateHP()
{
	RestoreHP(GetGenerateHealthPoint());
}

void ASidheRigelCharacter::SetTarget(AActor* _target)
{
	target = _target;
}

float ASidheRigelCharacter::GetRange()
{
	float res = 0;
	for (auto& value : range)
	{
		res += value.Value;
	}

	return res;
}

float ASidheRigelCharacter::GetAttackDamage()
{
	float res = 0;
	for (auto& value : attackDamage)
	{
		res += value.Value;
	}

	return res;
}

int32 ASidheRigelCharacter::GetCriticalRate()
{
	int32 res = 0;
	for (auto& value : criticalRate)
	{
		res += value.Value;
	}
	if (res > 100)	//MaxCriticalRate
	{
		res = 100;
	}

	return res;
}

int32 ASidheRigelCharacter::GetCriticalDamage()
{
	int32 res = 0;
	for (auto& value : criticalDamage)
	{
		res += value.Value;
	}
	
	return res;
}

float ASidheRigelCharacter::GetAttackSpeed()
{
	float res = 0;

	for (auto& value : attackSpeed)
	{
		res += value.Value;
	}
	if (res > 2.5f)	//MaxAttackSpeed
	{
		res = 2.5f;
	}

	return res;
}

float ASidheRigelCharacter::GetMaxHP()
{
	float res = 0;
	for (auto& value : MaxHP)
	{
		res += value.Value;
	}

	return res;
}

float ASidheRigelCharacter::GetGenerateHealthPoint()
{
	float res = 0;
	for (auto& value : generateHealthPoint)
	{
		res += value.Value;
	}

	return res;
}

int32 ASidheRigelCharacter::GetLifeSteal()
{
	int32 res = 0;
	for (auto& value : lifeSteal)
	{
		res += value.Value;
	}

	return res;
}

int32 ASidheRigelCharacter::GetProtectPower()
{
	int32 res = 0;
	for (auto& value : protectPower)
	{
		res += value.Value;
	}

	return res;
}

float ASidheRigelCharacter::GetDefencePoint()
{
	float res = 0.f;
	for (auto& value : defencePoint)
	{
		res += value.Value;
	}

	res *= (1 - GetDecreseDefence());

	return res;
}

void ASidheRigelCharacter::AddDecreseDefencePercent(FString name, float value, float time)
{
	decreseDefencePoint.Add(name, value);

	if (time == -1)
		return;

	FTimerHandle DebuffTimer;
	GetWorldTimerManager().SetTimer(DebuffTimer, FTimerDelegate::CreateLambda([=]()
		{
			if (decreseDefencePoint.Find(name))
			{
				decreseDefencePoint.Remove(name);
			}
		})
		, time, false);
}

float ASidheRigelCharacter::GetDecreseDefence()
{
	float res = 0.f;
	for (auto& value : decreseDefencePoint)
	{
		res += value.Value;
	}

	return res;
}

void ASidheRigelCharacter::InitProperty()
{
	range.Add("Debug", 500.f);
	attackDamage.Add("Debug", 5.f);
	attackSpeed.Add("Debug", 1.f);
	criticalRate.Add("Debug", 50);
	criticalDamage.Add("Debug", 50);
	MaxHP.Add("Debug", 100.f);
	generateHealthPoint.Add("Debug", 0.2f);
	lifeSteal.Add("Debug", 5.f);
	protectPower.Add("Debug", 20);

	defencePoint.Add("Debug", 100);

	currentHP = GetMaxHP();
}

void ASidheRigelCharacter::WaitAttackDelay()
{
	FTimerHandle AttackDelayTimer;
	GetWorldTimerManager().SetTimer(AttackDelayTimer, this, &ASidheRigelCharacter::ChangeAttackState, 1 / GetAttackSpeed(), false);
}

void ASidheRigelCharacter::ChangeAttackState()
{
	
}

void ASidheRigelCharacter::InitAttackProjectile()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Dummy/BP_DummyProjectile"));
	if (Projectile.Object)
	{
		baseProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}
}

void ASidheRigelCharacter::SpawnAttackProjectile()
{
	if (baseProjectileClass)
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
			ADummyProjectile* Projectile = World->SpawnActor<ADummyProjectile>(baseProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				InitProjectileProperty(Projectile);
			}
		}
	}
}

void ASidheRigelCharacter::InitProjectileProperty(ADummyProjectile* projectile)
{
	projectile->Target = target;
	projectile->projectileOwner = this;
	projectile->AttackDamage = GetAttackDamage();
	projectile->criticalRate = (float)GetCriticalRate() / 100.f;
	projectile->criticalDamage = (float)GetCriticalDamage() / 100.f + 1;
}

void ASidheRigelCharacter::LifeSteal(float damage)
{
	float _lifeSteal = (float)GetLifeSteal() / 100.f;
	
	RestoreHP(damage * _lifeSteal);
}

void ASidheRigelCharacter::Attack(AActor* _target)
{
	target = _target;
}

void ASidheRigelCharacter::Stun(float time)
{
}

void ASidheRigelCharacter::Stop(float time)
{
}

void ASidheRigelCharacter::Slow(float time, float value)
{
}

void ASidheRigelCharacter::Silence(float time)
{
}

void ASidheRigelCharacter::Airborne(float time)
{
}

void ASidheRigelCharacter::TakeDamage(float damage, AActor* damageCauser)
{
	currentHP -= damage;
	if (ASidheRigelCharacter* causerCharacter = Cast<ASidheRigelCharacter>(damageCauser))
	{
		causerCharacter->LifeSteal(damage);
	}
	if (currentHP <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Die"));
		currentHP = 0;
	}
}

void ASidheRigelCharacter::RestoreHP(float value)
{
	currentHP += value;
	
	float var_MaxHP = GetMaxHP();

	if (currentHP > var_MaxHP)
	{
		currentHP = var_MaxHP;
	}
}

void ASidheRigelCharacter::MoveVector(FVector Direction, float Force)
{
	moveDirection = Direction.GetSafeNormal();
	moveForce = Force;
	moveCnt = 0;
	IsMoveVectorTrue = true;
}

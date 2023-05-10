﻿// Copyright Epic Games, Inc. All Rights Reserved.

#include "SidheRigelCharacter.h"
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

	static ConstructorHelpers::FObjectFinder<UBlueprint> Projectile(TEXT("/Game/Dummy/BP_DummyProjectile"));
	if (Projectile.Object)
	{
		ProjectileClass = (UClass*)Projectile.Object->GeneratedClass;
	}

	//Character Property Initialize
	range.Add("Base", 500.f);
	attackDamage.Add("Base", 5.f);
	attackSpeed.Add("Base", 1.f);
	criticalRate.Add("Base", 50);
	criticalDamage.Add("Base", 50);
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("DEBUG")));

	bAttackDelay = false;
}

void ASidheRigelCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASidheRigelCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (target)
	{
		if (GetDistanceTo(target) <= GetRange())	//타겟이 사거리 내 범위에 속함
		{
			if (!bAttackDelay)
			{
				bAttackDelay = true;

				if (ProjectileClass)
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
						ADummyProjectile* Projectile = World->SpawnActor<ADummyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
						if (Projectile)
						{
							// Set the projectile's initial trajectory.
							Projectile->Target = target;
							Projectile->AttackDamage = GetAttackDamage();
							Projectile->criticalRate = (float)GetCriticalRate()/100.f;
							Projectile->criticalDamage = (float)GetCriticalDamage() / 100.f + 1;
						}
					}
				}
				FTimerHandle AttackDelayTimer;
				GetWorldTimerManager().SetTimer(AttackDelayTimer, this, &ASidheRigelCharacter::SetAttackDelayFalse, 1/GetAttackSpeed(), false);
			}
		}
		else												//타겟이 사거리 밖에 있음
		{
			FVector WorldDirection = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(WorldDirection, 1.f, false);
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

void ASidheRigelCharacter::UseSkill(AActor* _target)
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

void ASidheRigelCharacter::SetTarget(AActor* _target)
{
	target = _target;
}

float ASidheRigelCharacter::GetRange()
{
	float res = 0;
	for (auto value : range)
	{
		res += value.Value;
	}

	return res;
}

float ASidheRigelCharacter::GetAttackDamage()
{
	float res = 0;
	for (auto value : attackDamage)
	{
		res += value.Value;
	}

	return res;
}

int32 ASidheRigelCharacter::GetCriticalRate()
{
	int32 res = 0;
	for (auto value : criticalRate)
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
	for (auto value : criticalDamage)
	{
		res += value.Value;
	}
	
	return res;
}

float ASidheRigelCharacter::GetAttackSpeed()
{
	float res = 0;
	for (auto value : attackSpeed)
	{
		res += value.Value;
	}
	if (res > 2.5f)	//MaxAttackSpeed
	{
		res = 2.5f;
	}

	return res;
}

void ASidheRigelCharacter::SetAttackDelayFalse()
{
	bAttackDelay = false;
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

void ASidheRigelCharacter::TakeDamage(float damage)
{
}

void ASidheRigelCharacter::RestoreHP(float value)
{
}

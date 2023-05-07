// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdCharacter.h"
#include "SidheRigel/Character/Cold/Skill/ColdQProjectile.h"

// Sets default values
AColdCharacter::AColdCharacter()
{
	skillState = Null;

	static ConstructorHelpers::FObjectFinder<UBlueprint> QProjectile(TEXT("/Game/Heros/Cold/Skill/BP_ColdQProjectile"));
	if (QProjectile.Object)
	{
		QProjectileClass = (UClass*)QProjectile.Object->GeneratedClass;
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

void AColdCharacter::SkillOne()
{
	UE_LOG(LogTemp, Warning, TEXT("Cold Ready Q"));

	skillState = Q_Ready;
}

void AColdCharacter::SkillTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("Cold Ready W"));

	skillState = W_Ready;
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

void AColdCharacter::useSkill(AActor* target)
{
	switch (skillState)
	{
	case Null:
		UE_LOG(LogTemp, Warning, TEXT("Cold SkillState is Null"));
		break;
	case Q_Ready:
		UE_LOG(LogTemp, Warning, TEXT("Cold use Q"));

		if (QProjectileClass)
		{
			FVector MuzzleLocation = GetActorLocation();
			FRotator MuzzleRotation = GetActorRotation();

			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				for (int i = 0; i < QCount; i++)
				{
					// Spawn the projectile at the muzzle.
					AColdQProjectile* Projectile = World->SpawnActor<AColdQProjectile>(QProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Has Projectile!!")));
						// Set the projectile's initial trajectory.
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->Target = target;
					}
				}
			}
		}

		skillState = Null;
		break;
	case W_Ready:
		UE_LOG(LogTemp, Warning, TEXT("Cold use W"));
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdR2Projectile.h"
#include "Components/SplineMeshComponent.h"

#include "SidheRigel/Interface/Damagable.h"
#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/SidheRigelCharacter.h"
#include "SidheRigel/Character/Cold/Skill/ColdR2Laser.h"

// Sets default values
AColdR2Projectile::AColdR2Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!splineComponent)
	{
		splineComponent = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineComponent"));
		RootComponent = splineComponent;
	}

	static ConstructorHelpers::FObjectFinder<UClass>laserRef(TEXT("Blueprint'/Game/Heros/Cold/Skill/BP_ColdR2Laser.BP_ColdR2Laser_C'"));
	if (laserRef.Object)
	{
		laserClass = (UClass*)laserRef.Object;
	}
}

// Called when the game starts or when spawned
void AColdR2Projectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle R2ProjectileTimer;
	GetWorldTimerManager().SetTimer(R2ProjectileTimer,
		FTimerDelegate::CreateLambda([=]()
		{
			TSet<AActor*> overlapActors;
			splineComponent->GetOverlappingActors(overlapActors);

			for (auto& item : overlapActors)
			{
				if (ITeam* teamActor = Cast<ITeam>(item))
				{
					if (teamActor->GetTeam() != Cast<ITeam>(projectileOwner)->GetTeam())
					{
						if (IDamagable* damagableActor = Cast<IDamagable>(item))
						{
							damagableActor->TakeDamage(damage, projectileOwner);
						}
					}
				}
			}

			if (UWorld* World = GetWorld())
			{
				FActorSpawnParameters SpawnParams;
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(GetActorLocation());
				SpawnTransform.SetRotation(GetActorRotation().Quaternion());
				SpawnParams.Owner = projectileOwner;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AColdR2Laser* laser = World->SpawnActorDeferred<AColdR2Laser>(laserClass, SpawnTransform);
				if (laser)
				{
					laser->duration = 0.5f;
				}
				laser->FinishSpawning(SpawnTransform);
			}
			
			Destroy();
		}
	), duration, false);

}

// Called every frame
void AColdR2Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


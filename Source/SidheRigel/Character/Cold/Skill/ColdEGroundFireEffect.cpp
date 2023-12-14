// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdEGroundFireEffect.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "SidheRigel/Interface/Team.h"
#include "SidheRigel/Interface/Damagable.h"

// Sets default values
AColdEGroundFireEffect::AColdEGroundFireEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(damageArea);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}
}

// Called when the game starts or when spawned
void AColdEGroundFireEffect::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, this->GetActorLocation());
	
	FTimerHandle EEffectTimer;
	GetWorldTimerManager().SetTimer(EEffectTimer,
		FTimerDelegate::CreateLambda([=]()
			{
				Destroy();
			}
	), duration, false);
}

// Called every frame
void AColdEGroundFireEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	elapsedTime += DeltaTime;

	if (elapsedTime > 0.2f)
	{
		TSet<AActor*> overlapActors;
		CollisionComponent->GetOverlappingActors(overlapActors);

		for (auto& actor : overlapActors)
		{
			if (ITeam* teamActor = Cast<ITeam>(actor))
			{
				if (teamActor->GetTeam() != Cast<ITeam>(projectileOwner)->GetTeam())
				{
					if (IDamagable* damagableActor = Cast<IDamagable>(actor))
					{
						damagableActor->TakeDamage(damage, projectileOwner);
					}
				}
			}
		}

		elapsedTime = 0;
	}
}

